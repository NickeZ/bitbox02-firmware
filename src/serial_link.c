
enum hww_state {
    HWW_STATE_IDLE,
    HWW_STATE_ACCEPT,
    HWW_STATE_PROCESS,
    HWW_STATE_RESPOND,
};

enum serial_link_state {
    SERIAL_LINK_STATE_WAIT,
    SERIAL_LINK_STATE_ACCEPT,
    SERIAL_LINK_STATE_ESCAPE,
};

struct HWW {
    enum hww_state state;
    enum serial_link_state sl_state;
    uint8_t buf_in[64];
    uint8_t buf_in_len;
    uint8_t payload[64];
    uint8_t payload_len;
    uint8_t buf_out[64];
    uint16_t buf_out_len;
    uint16_t counter;
};

static void hww_init(struct HWW* self)
{
    self->state = HWW_STATE_IDLE;
    self->sl_state = SERIAL_LINK_STATE_WAIT;
    self->buf_in_len = 0;
    self->payload_len = 0;
    self->buf_out_len = 0;
    self->counter = 0;
}

static int hww_receive_bytes(struct HWW* self, const uint8_t* buf, uint16_t buf_len)
{
    // Already have data in buffer
    if (self->buf_in_len > 0) {
        return STATUS_ERR;
    }
    for (uint16_t i = 0; i < buf_len; i++) {
        self->buf_in[i] = buf[i];
    }
    self->buf_in_len = buf_len;
    return STATUS_OK;
}

static void hww_send_bytes(struct HWW* self)
{
    self->buf_out_len = 0;
}

#define SL_SOF 0x7E
#define SL_EOF 0x7E
#define SL_ESCAPE 0x7D
#define SL_XOR 0x20

static void hww_poll(struct HWW* self)
{
    event_registered |= HWW_READ;
    for (int i = 0; i < self->buf_in_len; i++) {
        self->payload[(self->payload_len)++] = self->buf_in[i];
    }
    self->buf_in_len = 0;

    for (int i = 0; i < self->payload_len; i++) {
        if (self->payload[i] == '\n') {
            util_log("%.*s", i, self->payload);
            self->payload_len = 0;
        }
    }
    // switch (self->state) {
    // case HWW_STATE_IDLE:
    //     self->state = HWW_STATE_ACCEPT;
    //     event_registered |= HWW_READ;
    //     break;
    // case HWW_STATE_ACCEPT:
    //     if (self->buf_in_len > 0) {
    //         for (int i = 0; i < self->buf_in_len; i++) {
    //             switch (self->sl_state) {
    //             case SERIAL_LINK_STATE_WAIT:
    //                 // Always reset on SOF
    //                 if (self->buf_in[i] == SL_SOF) {
    //                     if (self->payload_len >= 7) {
    //                         util_log("payload_len %d", self->payload_len);
    //                         self->state = HWW_STATE_PROCESS;
    //                     }
    //                     self->sl_state = SERIAL_LINK_STATE_ACCEPT;
    //                     self->payload_len = 0;
    //                 }
    //                 break;
    //             case SERIAL_LINK_STATE_ACCEPT:
    //                 // Always reset on SOF
    //                 if (self->buf_in[i] == SL_SOF) {
    //                     if (self->payload_len >= 7) {
    //                         util_log("payload_len %d", self->payload_len);
    //                         self->state = HWW_STATE_PROCESS;
    //                     }
    //                     self->payload_len = 0;
    //                 } else if (self->buf_in[i] == SL_ESCAPE) {
    //                     self->sl_state = SERIAL_LINK_STATE_ESCAPE;
    //                 } else {
    //                     self->payload[self->payload_len++] = self->buf_in[i];
    //                 }
    //                 break;
    //             case SERIAL_LINK_STATE_ESCAPE: {
    //                 uint8_t c = self->buf_in[i] ^ SL_XOR;
    //                 self->payload[self->payload_len++] = c;
    //                 self->sl_state = SERIAL_LINK_STATE_ACCEPT;
    //             } break;
    //             default:
    //                 break;
    //             }
    //         }

    //        // util_log("%d %d %d, payload: ", self->sl_state, self->payload_len,
    //        //             self->state);

    //        // for (int i = 0; i < self->payload_len; i++) {
    //        //   util_log("%02X", self->payload[i]);
    //        // }
    //        // util_log("");

    //        // debug_puts("got: ");
    //        // for (int i = 0; i < self->buf_in_len; i++) {
    //        //   util_log("%02X", self->buf_in[i]);
    //        // }
    //        // util_log("");
    //        self->buf_in_len = 0;
    //        event_registered |= HWW_READ;
    //    }
    //    break;
    // case HWW_STATE_PROCESS: {
    //    uint16_t len = self->payload[0] | self->payload[1] << 8;
    //    uint8_t typ = self->payload[2];
    //    uint32_t crc_frame = self->payload[3 + len] | self->payload[3 + len + 1] << 8 |
    //                         self->payload[3 + len + 2] << 16 | self->payload[3 + len + 3] << 24;
    //    crc_t crc = crc_init();
    //    crc = crc_update(crc, &self->payload[0], 3 + len);
    //    crc = crc_finalize(crc);

    //    util_log(
    //        "Got len: %d, type: %d, payload: %d, crc: %08X, crc final: %08X",
    //        len,
    //        typ,
    //        self->payload[3],
    //        (unsigned int)crc_frame,
    //        (unsigned int)crc);
    //    self->state = HWW_STATE_RESPOND;
    //} break;
    // case HWW_STATE_RESPOND:
    //    if (self->buf_out_len == 0) {
    //        util_log("ping");
    //        self->buf_out[0] = self->counter++;
    //        self->buf_out_len = 0;
    //        self->state = HWW_STATE_ACCEPT;
    //    }
    //    break;
    // default:
    //    break;
    //}
}
