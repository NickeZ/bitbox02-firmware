#include "firmware.h"
#include "ui/component.h"

// workflows must always go through these functions when the modify the screen stack

void workflow_process_push_component(struct app_state* app, component_t* component);

void workflow_process_pop_component(struct app_state* app);

void workflow_process(struct app_state* app);
