// Copyright 2023-2024 Shift Crypto AG
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_net.h>
#include <SDL_rwops.h>

#include "../build-build/src/version.h"
#include "hww.h"
#include "memory/bitbox02_smarteeprom.h"
#include "touch/gestures.h"
#include "usb/usb_packet.h"
#include "usb/usb_processing.h"
#include <fcntl.h>
#include <memory/memory.h>
#include <mock_memory.h>
#include <queue.h>
#include <random.h>
#include <rust/rust.h>
#include <screen.h>
#include <sd.h>
#include <stdio.h>
#include <ui/ugui/ugui.h>
#include <unistd.h>

#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/socket.h>

void rust_eh_personality(void);
void rust_eh_personality(void) {}

#define BUFFER_SIZE 1024

static const char* _simulator_version = "1.0.0";

static volatile sig_atomic_t sigint_called = false;
static TCPsocket servsock = NULL;
static SDLNet_SocketSet socketset = NULL;
static TCPsocket clientsock = NULL;

struct options {
    uint16_t port;
    bool headless;
};

#ifdef __MACH__
extern int _binary_bg_png_start __asm("section$start$__DATA_CONST$__bg_png");
extern int _binary_bg_png_end __asm("section$end$__DATA_CONST$__bg_png");
#else
extern int _binary_bg_png_start;
extern int _binary_bg_png_end;
#endif

static uint8_t* bg_data = (uint8_t*)&_binary_bg_png_start;

static size_t bg_data_len(void)
{
    return ((uint8_t*)&_binary_bg_png_end) - ((uint8_t*)&_binary_bg_png_start);
}

void send_usb_message_socket(void)
{
    const uint8_t* data = queue_pull(queue_hww_queue());
    while (data) {
        printf("TX: %s\n", util_dbg_hex(data, 64));
        if (!SDLNet_TCP_Send(clientsock, data, USB_HID_REPORT_OUT_SIZE)) {
            perror("ERROR, could not write to socket");
            exit(1);
        }
        data = queue_pull(queue_hww_queue());
    }
}

static void _int_handler(int _signum)
{
    sigint_called = true;
    SDLNet_TCP_Close(servsock);
}

static SDL_Window* window;
static SDL_Renderer* renderer;
static SDL_Texture* texture;
static SDL_Texture* texture_bg;

static uint32_t _display_buf[SCREEN_WIDTH * SCREEN_HEIGHT];

static void PixelFunc(UG_S16 x, UG_S16 y, UG_COLOR c)
{
    // White pixels are OPAQUE, Black pixels are completely transparent. This is blended on top
    // of the
    if (c) {
        _display_buf[(y * SCREEN_WIDTH) + x] = (c << 8) | (c << 16) | (c << 24) | 0xff;
    } else {
        _display_buf[(y * SCREEN_WIDTH) + x] = 0;
    }
}

static void MirrorFunc(bool mirror) {}

#define MARGIN 10
#define PADDING_TOP_BOTTOM 22
#define PADDING_LEFT 60
#define PADDING_RIGHT 35

static SDL_Rect content_area = {
    .x = MARGIN + PADDING_LEFT,
    .y = MARGIN + PADDING_TOP_BOTTOM,
    .w = SCREEN_WIDTH,
    .h = SCREEN_HEIGHT};
static SDL_Rect bg_area = {
    .x = MARGIN,
    .y = MARGIN,
    .w = SCREEN_WIDTH + PADDING_LEFT + PADDING_RIGHT,
    .h = SCREEN_HEIGHT + 2 * PADDING_TOP_BOTTOM};
static SDL_Rect slider_top_bg = {
    .x = MARGIN + PADDING_LEFT,
    .y = MARGIN / 2,
    .w = SCREEN_WIDTH,
    .h = MARGIN / 2};
static SDL_Rect slider_bottom_bg = {
    .x = MARGIN + PADDING_LEFT,
    .y = MARGIN + 2 * PADDING_TOP_BOTTOM + SCREEN_HEIGHT,
    .w = SCREEN_WIDTH,
    .h = MARGIN / 2};
static SDL_Rect pinch_bg = {
    .x = MARGIN + PADDING_LEFT + 128 + PADDING_RIGHT,
    .y = MARGIN + PADDING_TOP_BOTTOM + SCREEN_HEIGHT / 3,
    .w = MARGIN / 2,
    .h = SCREEN_HEIGHT / 3};

static void _init_sdl(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    window = SDL_CreateWindow(
        "BitBox simulator",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH + 2 * MARGIN + PADDING_LEFT + PADDING_RIGHT,
        SCREEN_HEIGHT + 2 * MARGIN + 2 * PADDING_TOP_BOTTOM,
        0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    texture = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);

    // SDL_SetTextureBlendMode(texture_bg, SDL_BLENDMODE_ADD);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    // Set draw color to white. (clear will fill with the latest selected color)
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE);
}

static void _init_sdl_img(void)
{
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        fprintf(stderr, "Failed to initialize image loader");
    }
    SDL_RWops* bg = SDL_RWFromConstMem(bg_data, bg_data_len());
    texture_bg = IMG_LoadTexture_RW(renderer, bg, 0);
    if (!texture_bg) {
        SDL_Log("Couldn't load simulator background: %s\n", SDL_GetError());
    }
}

static void _init_sdl_net(struct options* options)
{
    if (SDLNet_Init() < 0) {
        exit(1);
    }

    // We can only handle 1 client at a time. Need space for 1 server socket and 1 client socket
    socketset = SDLNet_AllocSocketSet(1 + 1);

    // IPaddress addr = {.host = INADDR_ANY, .port = options->port};
    IPaddress addr;
    SDLNet_ResolveHost(&addr, NULL, options->port);
    servsock = SDLNet_TCP_Open(&addr);
    SDLNet_TCP_AddSocket(socketset, servsock);

    printf("Listening on %x, %d\n", addr.host, addr.port);
}

void ClearFunc(void)
{
    memset(_display_buf, 0, sizeof(_display_buf));
}

static void _init_hww(void)
{
    screen_init(PixelFunc, MirrorFunc, ClearFunc);
    screen_splash();

    // BitBox02 simulation initialization
    usb_processing_init();
    printf("USB setup success\n");

    hww_setup();
    printf("HWW setup success\n");

    bool sd_success = sd_format();
    printf("Sd card setup %s\n", sd_success ? "success" : "failed");
    if (!sd_success) {
        perror("ERROR, sd card setup failed");
        exit(1);
    }

    mock_memory_factoryreset();
    memory_interface_functions_t ifs = {
        .random_32_bytes = random_32_bytes_mcu,
    };
    bool memory_success = memory_setup(&ifs);
    printf("Memory setup %s\n", memory_success ? "success" : "failed");
    if (!memory_success) {
        perror("ERROR, memory setup failed");
        exit(1);
    }

    smarteeprom_bb02_config();
    bitbox02_smarteeprom_init();
}

static void _parse_args(int argc, char* argv[], struct options* options)
{
    // Default options
    options->port = 15423;
    options->headless = true;

    struct option long_options[] = {
        {"port", required_argument, 0, 'p'},
        {"version", no_argument, 0, 'v'},
        {"gui", no_argument, 0, 'g'},
        {0, 0, 0, 0}};

    int opt;
    while ((opt = getopt_long(argc, argv, "", long_options, NULL)) != -1) {
        switch (opt) {
        case 'p':
            options->port = atoi(optarg);
            break;
        case 'v':
            printf(
                "bitbox02-multi-%s-simulator%s-linux-amd64\n",
                DIGITAL_BITBOX_VERSION_SHORT,
                _simulator_version);
            exit(0);
        case 'g':
            options->headless = false;
            break;
        default:
            fprintf(stderr, "Usage: %s --port <port number>\n", argv[0]);
            exit(1);
        }
    }
}

static void _handle_client(void)
{
    // BitBox02 firmware loop
    uint8_t input[BUFFER_SIZE];
    int temp_len;
    if (SDLNet_TCP_Recv(clientsock, input, USB_HID_REPORT_OUT_SIZE) <= 0) {
        // conection closed
        SDLNet_TCP_DelSocket(socketset, clientsock);
        SDLNet_TCP_Close(clientsock);
        clientsock = NULL;
        printf("client disconnected\n");
        return;
    }
    printf("RX: %s\n", util_dbg_hex(input, 64));

    usb_packet_process((const USB_FRAME*)input);
}

static void _handle_server(void)
{
    TCPsocket newsock;

    newsock = SDLNet_TCP_Accept(servsock);
    if (newsock == NULL) {
        return;
    }

    if (clientsock != NULL) {
        fprintf(stderr, "Already connected\n");
        return;
    }

    clientsock = newsock;
    SDLNet_TCP_AddSocket(socketset, clientsock);
    printf("Socket connection setup success\n");
}

struct Slider {
    bool active;
    uint16_t position;
    bool pinch;
};

int main(int argc, char* argv[])
{
    signal(SIGINT, _int_handler);

    struct options options;
    _parse_args(argc, argv, &options);
    _init_sdl();
    _init_sdl_img();
    _init_sdl_net(&options);
    _init_hww();

    SDL_Event e;
    bool quit = false;
    int last_ticks = 0;
    struct Slider slider_top = {0};
    struct Slider slider_bottom = {0};

    gestures_slider_data_t slider_data;

    while (!quit) {
        /* Check input events */
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
                break;
            }

            if (e.type == SDL_MOUSEMOTION) {
                slider_data.diff = e.motion.xrel;
                slider_data.position = (e.motion.x - MARGIN - PADDING_LEFT) * 2;
                slider_data.velocity = 0;
                if (e.motion.y < MARGIN / 2) {
                    if (slider_top.active) {
                        // printf("top tap\n");
                        slider_top.active = false;
                        event_t event = {.data = &slider_data, .id = EVENT_TOP_SHORT_TAP};
                        emit_event(&event);
                    }
                } else if (e.motion.y < MARGIN) {
                    slider_top.active = true;
                } else if (
                    e.motion.y > MARGIN + PADDING_TOP_BOTTOM + SCREEN_HEIGHT / 3 &&
                    e.motion.y < MARGIN + PADDING_TOP_BOTTOM + SCREEN_HEIGHT * 2 / 3) {
                    if (e.motion.x > MARGIN + PADDING_LEFT + SCREEN_WIDTH + PADDING_RIGHT) {
                        slider_top.pinch = true;
                    }
                } else if (e.motion.y > MARGIN + 2 * PADDING_TOP_BOTTOM + 64 + MARGIN / 2) {
                    if (slider_bottom.active) {
                        // printf("bottom tap\n");
                        slider_bottom.active = false;
                        event_t event = {.data = &slider_data, .id = EVENT_BOTTOM_SHORT_TAP};
                        emit_event(&event);
                    }
                } else if (e.motion.y > MARGIN + 2 * PADDING_TOP_BOTTOM + 64) {
                    slider_bottom.active = true;
                } else {
                    slider_top.pinch = false;
                }
            }
        }

        SDLNet_CheckSockets(socketset, 10);

        /* Check for data from connected client */
        if (SDLNet_SocketReady(clientsock)) {
            _handle_client();
        }

        /* Send data if we have any */
        send_usb_message_socket();

        /* Check for new connections */
        if (SDLNet_SocketReady(servsock)) {
            _handle_server();
        }

        rust_workflow_spin();
        rust_async_usb_spin();
        usb_processing_process(usb_processing_hww());

        // Only refresh screen with 60hz
        uint32_t ticks = SDL_GetTicks();
        if (ticks - last_ticks >= 1000 / 60) {
            if (slider_top.pinch) {
                slider_data.position = SLIDER_POSITION_TWO_THIRD + 1;
                event_t event = {.data = &slider_data, .id = EVENT_TOP_CONTINUOUS_TAP};
                emit_event(&event);
                event.id = EVENT_BOTTOM_CONTINUOUS_TAP;
                emit_event(&event);
            }
            // All logic depends on the screen rate right now...
            screen_process();

            /* Update simulated screen */
            last_ticks = SDL_GetTicks();
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture_bg, NULL, &bg_area);
            // SDL_RenderFillRect(renderer, NULL);
            SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(renderer, &slider_top_bg);
            SDL_RenderFillRect(renderer, &slider_bottom_bg);
            SDL_RenderFillRect(renderer, &pinch_bg);
            SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE);
            SDL_UpdateTexture(texture, NULL, _display_buf, SCREEN_WIDTH * sizeof(uint32_t));
            SDL_RenderCopy(renderer, texture, NULL, &content_area);
            SDL_RenderPresent(renderer);
        }

        if (sigint_called) {
            printf("\nGot Ctrl-C, exiting\n");
            break;
        }
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
