// SPDX-License-Identifier: Apache-2.0

extern crate alloc;

use alloc::boxed::Box;
use core::ffi::{c_char, c_void};

struct StatusData {
    status_success: bool,
}

static COMPONENT_FUNCTIONS: bitbox02_sys::component_functions_t =
    bitbox02_sys::component_functions_t {
        cleanup: Some(cleanup),
        render: Some(render),
        on_event: None,
    };

unsafe extern "C" fn cleanup(component: *mut bitbox02_sys::component_t) {
    assert!(!component.is_null());

    let component_ref = unsafe { &mut *component };
    for child in component_ref
        .sub_components
        .sub_components
        .iter()
        .take(component_ref.sub_components.amount as usize)
    {
        assert!(!child.is_null());
        let child_ref = unsafe { &mut **child };
        let functions = unsafe { &*child_ref.f };
        unsafe { functions.cleanup.unwrap()(*child) };
    }

    if !component_ref.data.is_null() {
        unsafe {
            drop(Box::from_raw(component_ref.data as *mut StatusData));
        }
    }
    unsafe {
        drop(Box::from_raw(component));
    }
}

unsafe extern "C" fn render(component: *mut bitbox02_sys::component_t) {
    assert!(!component.is_null());

    let component_ref = unsafe { &mut *component };
    let data = unsafe { &*(component_ref.data as *const StatusData) };
    let height = 10;
    let x = bitbox02_sys::SCREEN_WIDTH as i32 / 6 * 5;
    let y = bitbox02_sys::SCREEN_HEIGHT as i32 / 2 - height / 2;
    if data.status_success {
        unsafe { bitbox02_sys::image_checkmark(x, y, height) };
    } else {
        unsafe { bitbox02_sys::image_cross(x, y, height) };
    }
    render_subcomponents(component_ref);
}

fn render_subcomponents(component: &mut bitbox02_sys::component_t) {
    for child in component
        .sub_components
        .sub_components
        .iter()
        .take(component.sub_components.amount as usize)
    {
        assert!(!child.is_null());
        let child_ref = unsafe { &mut **child };
        if child_ref.disabled {
            continue;
        }
        let functions = unsafe { &*child_ref.f };
        unsafe { functions.render.unwrap()(*child) };
    }
}

unsafe fn add_sub_component(
    parent: *mut bitbox02_sys::component_t,
    child: *mut bitbox02_sys::component_t,
) {
    assert!(!parent.is_null());
    assert!(!child.is_null());

    let parent_ref = unsafe { &mut *parent };
    let amount = parent_ref.sub_components.amount as usize;
    if amount + 1 >= parent_ref.sub_components.sub_components.len() {
        panic!("Not enough memory to add sub component");
    }
    parent_ref.sub_components.sub_components[amount] = child;
    parent_ref.sub_components.amount += 1;

    let child_ref = unsafe { &mut *child };
    child_ref.parent = parent;
}

/// cbindgen:ignore
#[unsafe(no_mangle)]
pub unsafe extern "C" fn status_create(
    text: *const c_char,
    status_success: bool,
) -> *mut bitbox02_sys::component_t {
    assert!(!text.is_null());

    let data = Box::new(StatusData { status_success });
    let mut component = Box::new(bitbox02_sys::component_t {
        f: &COMPONENT_FUNCTIONS,
        dimension: bitbox02_sys::dimension_t {
            width: bitbox02_sys::SCREEN_WIDTH as _,
            height: bitbox02_sys::SCREEN_HEIGHT as _,
        },
        position: Default::default(),
        data: Box::into_raw(data).cast::<c_void>(),
        sub_components: Default::default(),
        parent: core::ptr::null_mut(),
        emit_without_release: false,
        disabled: false,
    });

    let component_ptr = component.as_mut() as *mut _;
    let label = unsafe {
        bitbox02_sys::label_create(
            text,
            core::ptr::null(),
            bitbox02_sys::screen_position_t_LEFT_CENTER,
            component_ptr,
        )
    };
    unsafe { add_sub_component(component_ptr, label) };

    Box::into_raw(component)
}
