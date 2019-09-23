#include "workflow_process.h"
#include "ui/screen_stack.h"

void workflow_process_push_component(struct app_state* app, component_t* component) {
    ui_screen_stack_push(component);
    app->gesture_detect_reset = true;
}

void workflow_process_pop_component(struct app_state* app) {
    ui_screen_stack_pop();
    app->gesture_detect_reset = true;
}

void workflow_process(struct app_state* app) {
    // Step forward workflow

}
