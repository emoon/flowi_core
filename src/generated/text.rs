// This file is auto-generated by api_gen. DO NOT EDIT!

#[allow(unused_imports)]
use crate::manual::{get_last_error, Color, FlString, Result};

#[allow(unused_imports)]
use bitflags::bitflags;

#[allow(unused_imports)]
use crate::math_data::*;

#[repr(C)]
pub struct TextFfiApi {
    pub(crate) data: *const core::ffi::c_void,
    pub(crate) calc_size:
        unsafe extern "C" fn(data: *const core::ffi::c_void, text: FlString) -> Vec2,
    pub(crate) bullet: unsafe extern "C" fn(data: *const core::ffi::c_void, text: FlString),
    pub(crate) label:
        unsafe extern "C" fn(data: *const core::ffi::c_void, label: FlString, text: FlString),
    pub(crate) show_color:
        unsafe extern "C" fn(data: *const core::ffi::c_void, color: Color, text: FlString),
    pub(crate) show: unsafe extern "C" fn(data: *const core::ffi::c_void, text: FlString),
    pub(crate) text_disabled: unsafe extern "C" fn(data: *const core::ffi::c_void, text: FlString),
}

#[cfg(feature = "static")]
extern "C" {
    pub fn fl_text_calc_size_impl(data: *const core::ffi::c_void, text: FlString) -> Vec2;
    pub fn fl_text_bullet_impl(data: *const core::ffi::c_void, text: FlString);
    pub fn fl_text_label_impl(data: *const core::ffi::c_void, label: FlString, text: FlString);
    pub fn fl_text_show_color_impl(data: *const core::ffi::c_void, color: Color, text: FlString);
    pub fn fl_text_show_impl(data: *const core::ffi::c_void, text: FlString);
    pub fn fl_text_text_disabled_impl(data: *const core::ffi::c_void, text: FlString);
}

#[no_mangle]
pub static mut g_flowi_text_api: *const TextFfiApi = std::ptr::null_mut();

#[repr(C)]
#[derive(Debug)]
pub struct Text {
    _dummy: u32,
}

impl Text {
    /// Calculate the size of a text string in pixels
    pub fn calc_size(text: &str) -> Vec2 {
        unsafe {
            let _api = &*g_flowi_text_api;
            #[cfg(feature = "static")]
            let ret_val = fl_text_calc_size_impl(_api.data, FlString::new(text));
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            let ret_val = (_api.calc_size)(_api.data, FlString::new(text));
            ret_val
        }
    }

    /// Bullet text
    pub fn bullet(text: &str) {
        unsafe {
            let _api = &*g_flowi_text_api;
            #[cfg(feature = "static")]
            fl_text_bullet_impl(_api.data, FlString::new(text));
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.bullet)(_api.data, FlString::new(text));
        }
    }

    /// Draw basic text
    pub fn label(label: &str, text: &str) {
        unsafe {
            let _api = &*g_flowi_text_api;
            #[cfg(feature = "static")]
            fl_text_label_impl(_api.data, FlString::new(label), FlString::new(text));
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.label)(_api.data, FlString::new(label), FlString::new(text));
        }
    }

    /// Draw basic text with a color
    pub fn show_color(color: Color, text: &str) {
        unsafe {
            let _api = &*g_flowi_text_api;
            #[cfg(feature = "static")]
            fl_text_show_color_impl(_api.data, color, FlString::new(text));
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.show_color)(_api.data, color, FlString::new(text));
        }
    }

    /// Show basic text
    pub fn show(text: &str) {
        unsafe {
            let _api = &*g_flowi_text_api;
            #[cfg(feature = "static")]
            fl_text_show_impl(_api.data, FlString::new(text));
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.show)(_api.data, FlString::new(text));
        }
    }

    /// Draw text disabled
    pub fn text_disabled(text: &str) {
        unsafe {
            let _api = &*g_flowi_text_api;
            #[cfg(feature = "static")]
            fl_text_text_disabled_impl(_api.data, FlString::new(text));
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.text_disabled)(_api.data, FlString::new(text));
        }
    }
}
