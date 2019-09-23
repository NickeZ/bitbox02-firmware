#include <stdint.h>
#include <stdbool.h>

struct app_state {
    // Global counter of how many iterations event loop has made.
    uint32_t counter;
    // When this flag is set the gesture detecting module will reset itself.
    bool gesture_detect_reset;
    // When this flag is set, USB frames should not initiate new workflows.
    bool waiting_on_user_input;
};
