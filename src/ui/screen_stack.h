// Copyright 2019 Shift Cryptosecurity AG
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

#ifndef _UI_STACK_H_
#define _UI_STACK_H_

#include "component.h"
#include "event.h"

/**
 * Get the top-most element from the "screen" stack
 */
component_t* ui_screen_stack_top(void);

/**
 * Push element onto "screen" stack.
 */
void ui_screen_stack_push(component_t* component);

/**
 * Moves element from "screen" stack to "pop" stack
 */
void ui_screen_stack_pop(void);

/**
 * Pop on complete "screen" stack.
 */
void ui_screen_stack_pop_all(void);

/**
 * Pop + push
 */
void ui_screen_stack_switch(component_t* component);

/**
 * Frees all components that are on the "pop" stack.
 *
 * @return Number of components freed
 */
int ui_screen_stack_cleanup(void);

#endif
