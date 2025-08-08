use sdl2::event::{Event, EventSender};
use sdl2::image::LoadTexture;
use sdl2::keyboard::Keycode;
use sdl2::pixels::Color;
use sdl2::rect::Rect;
use std::io::{Read, Write};
use std::net::{TcpListener, TcpStream};
use std::sync::{
    Arc,
    atomic::{AtomicU32, Ordering},
    mpsc,
};
use std::thread;
use std::time::Duration;

#[derive(Debug)]
struct NewConnectionEvent {}

#[derive(Debug)]
struct HWWDataEvent {}

static BG: &[u8; 325362] = include_bytes!("../../simulator/bg.png");

use bitbox02::bindings::{SCREEN_HEIGHT, SCREEN_WIDTH, UG_COLOR};

const MARGIN: u32 = 10;
const PADDING_TOP_BOTTOM: u32 = 22;
const PADDING_LEFT: u32 = 60;
const PADDING_RIGHT: u32 = 35;

pub fn handle_stream(mut stream: TcpStream, es: EventSender, counter: Arc<AtomicU32>) {
    let mut buf = [0; 128];
    while let Ok(len) = stream.read(&mut buf) {
        if len == 0 {
            // Client disconnected
            break;
        }
        let event = HWWDataEvent {};
        es.push_custom_event(event).unwrap();
        if stream.write_all(&buf[..len]).is_err() {
            // Client disconnected while writing
            break;
        }
    }
    println!("Disconnected");
    counter.fetch_sub(1, Ordering::SeqCst);
}

pub fn network_task(es: EventSender, evs: mpsc::Receiver<EventSender>) {
    let listener = TcpListener::bind("0.0.0.0:15423").unwrap();

    // Use this counter to ensure that we only connect to a single client.
    let counter = Arc::new(AtomicU32::new(0));
    for stream in listener.incoming() {
        if counter.fetch_add(1, Ordering::SeqCst) == 0 {
            let event = NewConnectionEvent {};
            es.push_custom_event(event).unwrap();
            let stream = stream.unwrap();
            thread::spawn({
                let es = evs.recv().unwrap();
                let counter = counter.clone();
                move || handle_stream(stream, es, counter)
            });
        } else {
            println!("Busy, won't accept new clients");
            counter.fetch_sub(1, Ordering::SeqCst);
        }
    }
}

static mut DISPLAY_BUF: [u32; (SCREEN_WIDTH * SCREEN_HEIGHT) as usize] =
    [0; (SCREEN_WIDTH * SCREEN_HEIGHT) as usize];

unsafe extern "C" fn pixel_fn(x: i16, y: i16, c: UG_COLOR) {
    let ptr: *mut _ = unsafe { &raw mut DISPLAY_BUF[0] };
    let c = c as u32;
    let offset = ((y * SCREEN_WIDTH as i16) + x) as usize;

    // White pixels are OPAQUE, Black pixels are completely transparent.
    if c != 0 {
        unsafe { *ptr.add(offset) = (c << 8) | (c << 16) | (c << 24) | 0xff }
    } else {
        unsafe { *ptr.add(offset) = 0 }
    }
}

unsafe extern "C" fn clear_fn() {
    let ptr: *mut _ = unsafe { &raw mut DISPLAY_BUF[0] };
    for _ in 0..SCREEN_WIDTH * SCREEN_HEIGHT {
        unsafe { *ptr = 0 }
    }
}

unsafe extern "C" fn mirror_fn(_: bool) {}

fn init_hww() -> bool {
    unsafe { bitbox02::bindings::screen_init(Some(pixel_fn), Some(mirror_fn), Some(clear_fn)) };
    unsafe { bitbox02::bindings::screen_splash() };

    // BitBox02 simulation initialization
    unsafe { bitbox02::bindings::usb_processing_init() };
    println!("USB setup success");

    unsafe { bitbox02::bindings::hww_setup() };
    println!("HWW setup success");

    let sd_success = unsafe { bitbox02::bindings::sd_format() };
    if !sd_success {
        eprintln!("ERROR, sd card setup failed");
        return false;
    }

    println!("Sd card setup: success");

    bitbox02::testing::mock_memory();
    println!("Memory setup: success");

    unsafe { bitbox02::bindings::smarteeprom_bb02_config() };
    unsafe { bitbox02::bindings::bitbox02_smarteeprom_init() };
    true
}

pub fn main() -> Result<(), i32> {
    let sdl_context = sdl2::init().unwrap();

    if !init_hww() {
        return Err(1);
    }

    let ev = sdl_context.event().unwrap();
    let (evs_sender, evs_receiver) = mpsc::channel();
    ev.register_custom_event::<NewConnectionEvent>().unwrap();
    ev.register_custom_event::<HWWDataEvent>().unwrap();
    let es = ev.event_sender();
    thread::spawn(|| network_task(es, evs_receiver));

    let video_subsystem = sdl_context.video().unwrap();

    let window = video_subsystem
        .window(
            "BitBox02 simulator",
            SCREEN_WIDTH + 2 * MARGIN + PADDING_LEFT + PADDING_RIGHT,
            SCREEN_HEIGHT + 2 * MARGIN + 2 * PADDING_TOP_BOTTOM,
        )
        .position_centered()
        .build()
        .unwrap();

    let mut canvas = window.into_canvas().build().unwrap();

    let tc = canvas.texture_creator();
    let bg = tc.load_texture_bytes(BG).unwrap();

    let _content_area = Rect::new(
        (MARGIN + PADDING_LEFT) as i32,
        (MARGIN + PADDING_TOP_BOTTOM) as i32,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
    );
    let bg_area = Rect::new(
        MARGIN as i32,
        MARGIN as i32,
        SCREEN_WIDTH + PADDING_LEFT + PADDING_RIGHT,
        SCREEN_HEIGHT + 2 * PADDING_TOP_BOTTOM,
    );
    let _slider_top_bg = Rect::new(
        (MARGIN + PADDING_LEFT) as i32,
        (MARGIN / 2) as i32,
        SCREEN_WIDTH,
        MARGIN / 2,
    );
    let _slider_bottom_bg = Rect::new(
        (MARGIN + PADDING_LEFT) as i32,
        (MARGIN + 2 * PADDING_TOP_BOTTOM + SCREEN_HEIGHT) as i32,
        SCREEN_WIDTH,
        MARGIN / 2,
    );
    let _pinch_bg = Rect::new(
        (MARGIN + PADDING_LEFT + 128 + PADDING_RIGHT) as i32,
        (MARGIN + PADDING_TOP_BOTTOM + SCREEN_HEIGHT / 3) as i32,
        MARGIN / 2,
        SCREEN_HEIGHT / 3,
    );

    canvas.set_draw_color(Color::RGB(0, 255, 255));
    canvas.clear();
    canvas.present();
    let mut event_pump = sdl_context.event_pump().unwrap();
    let mut i = 0;
    'running: loop {
        i = (i + 1) % 255;
        canvas.set_draw_color(Color::RGB(i, 64, 255 - i));
        canvas.clear();
        for event in event_pump.poll_iter() {
            match event {
                Event::Quit { .. }
                | Event::KeyDown {
                    keycode: Some(Keycode::Escape),
                    ..
                } => break 'running,
                Event::User { .. } => {
                    if let Some(e) = event.as_user_event_type::<NewConnectionEvent>() {
                        println!("New connection {e:?}");
                        evs_sender.send(ev.event_sender()).unwrap();
                    } else if let Some(e) = event.as_user_event_type::<HWWDataEvent>() {
                        println!("HWW data {e:?}");
                    }
                }
                _ => {}
            }
        }
        // The rest of the game loop goes here...
        canvas
            .copy(&bg, None, bg_area)
            .expect("Couldn't copy to canvas");

        canvas.present();
        ::std::thread::sleep(Duration::new(0, 1_000_000_000u32 / 60));
    }
    Ok(())
}
