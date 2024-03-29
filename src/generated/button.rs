// This file is auto-generated by api_gen. DO NOT EDIT!

#[allow(unused_imports)]
use crate::manual::{get_last_error, Color, FlString, Result};

#[allow(unused_imports)]
use bitflags::bitflags;

#[allow(unused_imports)]
use crate::math_data::*;

#[allow(unused_imports)]
use crate::image::*;

#[repr(C)]
pub struct ButtonFfiApi {
    pub(crate) data: *const core::ffi::c_void,
    pub(crate) regular:
        unsafe extern "C" fn(data: *const core::ffi::c_void, label: FlString) -> bool,
    pub(crate) regular_size:
        unsafe extern "C" fn(data: *const core::ffi::c_void, label: FlString, size: Vec2) -> bool,
    pub(crate) small: unsafe extern "C" fn(data: *const core::ffi::c_void, label: FlString) -> bool,
    pub(crate) invisible: unsafe extern "C" fn(
        data: *const core::ffi::c_void,
        label: FlString,
        size: Vec2,
        flags: ButtonFlags,
    ) -> bool,
    pub(crate) check_box: unsafe extern "C" fn(
        data: *const core::ffi::c_void,
        label: FlString,
        state: *mut bool,
    ) -> bool,
    pub(crate) radio:
        unsafe extern "C" fn(data: *const core::ffi::c_void, label: FlString, state: bool) -> bool,
    pub(crate) bullet: unsafe extern "C" fn(data: *const core::ffi::c_void),
    pub(crate) image_with_text:
        unsafe extern "C" fn(data: *const core::ffi::c_void, image: u64, label: FlString) -> bool,
}

#[cfg(feature = "static")]
extern "C" {
    pub fn fl_button_regular_impl(data: *const core::ffi::c_void, label: FlString) -> bool;
    pub fn fl_button_regular_size_impl(
        data: *const core::ffi::c_void,
        label: FlString,
        size: Vec2,
    ) -> bool;
    pub fn fl_button_small_impl(data: *const core::ffi::c_void, label: FlString) -> bool;
    pub fn fl_button_invisible_impl(
        data: *const core::ffi::c_void,
        label: FlString,
        size: Vec2,
        flags: ButtonFlags,
    ) -> bool;
    pub fn fl_button_check_box_impl(
        data: *const core::ffi::c_void,
        label: FlString,
        state: *mut bool,
    ) -> bool;
    pub fn fl_button_radio_impl(
        data: *const core::ffi::c_void,
        label: FlString,
        state: bool,
    ) -> bool;
    pub fn fl_button_bullet_impl(data: *const core::ffi::c_void);
    pub fn fl_button_image_with_text_impl(
        data: *const core::ffi::c_void,
        image: u64,
        label: FlString,
    ) -> bool;
}

#[no_mangle]
pub static mut g_flowi_button_api: *const ButtonFfiApi = std::ptr::null_mut();

bitflags! {
#[repr(C)]
pub struct ButtonFlags : u32 {
    /// Default flags
    const NONE = 0;
    /// React on left mouse button (default)
    const MOUSE_BUTTON_LEFT = 1 << 0;
    /// React on right mouse button
    const MOUSE_BUTTON_RIGHT = 1 << 1;
    /// React on center mouse button
    const MOUSE_BUTTON_MIDDLED = 1 << 2;
}}

#[repr(C)]
#[derive(Debug)]
pub struct Button {
    _dummy: u32,
}

impl Button {
    /// Show a regular push button
    pub fn regular(label: &str) -> bool {
        unsafe {
            let _api = &*g_flowi_button_api;
            #[cfg(feature = "static")]
            let ret_val = fl_button_regular_impl(_api.data, FlString::new(label));
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            let ret_val = (_api.regular)(_api.data, FlString::new(label));
            ret_val
        }
    }

    /// Show a regular push button with a specific size
    pub fn regular_size(label: &str, size: Vec2) -> bool {
        unsafe {
            let _api = &*g_flowi_button_api;
            #[cfg(feature = "static")]
            let ret_val = fl_button_regular_size_impl(_api.data, FlString::new(label), size);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            let ret_val = (_api.regular_size)(_api.data, FlString::new(label), size);
            ret_val
        }
    }

    /// Show a regular push button without any frame padding.
    pub fn small(label: &str) -> bool {
        unsafe {
            let _api = &*g_flowi_button_api;
            #[cfg(feature = "static")]
            let ret_val = fl_button_small_impl(_api.data, FlString::new(label));
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            let ret_val = (_api.small)(_api.data, FlString::new(label));
            ret_val
        }
    }

    /// Invisible button that allows custom using drawing, but still acts like a button.
    pub fn invisible(label: &str, size: Vec2, flags: ButtonFlags) -> bool {
        unsafe {
            let _api = &*g_flowi_button_api;
            #[cfg(feature = "static")]
            let ret_val = fl_button_invisible_impl(_api.data, FlString::new(label), size, flags);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            let ret_val = (_api.invisible)(_api.data, FlString::new(label), size, flags);
            ret_val
        }
    }

    /// Button with a check box state
    pub fn check_box(label: &str, state: &mut bool) -> bool {
        unsafe {
            let _api = &*g_flowi_button_api;
            #[cfg(feature = "static")]
            let ret_val = fl_button_check_box_impl(_api.data, FlString::new(label), state as _);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            let ret_val = (_api.check_box)(_api.data, FlString::new(label), state as _);
            ret_val
        }
    }

    /// Radio button
    pub fn radio(label: &str, state: bool) -> bool {
        unsafe {
            let _api = &*g_flowi_button_api;
            #[cfg(feature = "static")]
            let ret_val = fl_button_radio_impl(_api.data, FlString::new(label), state);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            let ret_val = (_api.radio)(_api.data, FlString::new(label), state);
            ret_val
        }
    }

    /// TODO: Document
    pub fn bullet() {
        unsafe {
            let _api = &*g_flowi_button_api;
            #[cfg(feature = "static")]
            fl_button_bullet_impl(_api.data);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.bullet)(_api.data);
        }
    }

    /// TODO: Document
    pub fn image_with_text(image: Image, label: &str) -> bool {
        unsafe {
            let _api = &*g_flowi_button_api;
            #[cfg(feature = "static")]
            let ret_val =
                fl_button_image_with_text_impl(_api.data, image.handle, FlString::new(label));
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            let ret_val = (_api.image_with_text)(_api.data, image.handle, FlString::new(label));
            ret_val
        }
    }
}
