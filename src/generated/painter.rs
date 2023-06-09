// This file is auto-generated by api_gen. DO NOT EDIT!

#[allow(unused_imports)]
use crate::manual::{get_last_error, Color, FlString, Result};

#[allow(unused_imports)]
use bitflags::bitflags;

#[allow(unused_imports)]
use crate::layout::*;

#[allow(unused_imports)]
use crate::math_data::*;

#[allow(unused_imports)]
use crate::image::*;

#[repr(C)]
pub struct PainterFfiApi {
    pub(crate) data: *const core::ffi::c_void,
    pub(crate) set_layer: unsafe extern "C" fn(data: *const core::ffi::c_void, layer: PainterLayer),
    pub(crate) draw_line: unsafe extern "C" fn(
        data: *const core::ffi::c_void,
        p1: Vec2,
        p2: Vec2,
        color: Color,
        thickness: f32,
    ),
    pub(crate) draw_rect: unsafe extern "C" fn(
        data: *const core::ffi::c_void,
        p1: Vec2,
        p2: Vec2,
        color: Color,
        rounding: f32,
    ),
    pub(crate) draw_rect_filled: unsafe extern "C" fn(
        data: *const core::ffi::c_void,
        p1: Vec2,
        p2: Vec2,
        color: Color,
        rounding: f32,
    ),
    pub(crate) draw_rect_filled_gradient: unsafe extern "C" fn(
        data: *const core::ffi::c_void,
        p1: Vec2,
        p2: Vec2,
        left: Color,
        right: Color,
        btm_right: Color,
        btm_left: Color,
    ),
}

#[cfg(any(feature = "static", feature = "tundra"))]
extern "C" {
    fn fl_painter_set_layer_impl(data: *const core::ffi::c_void, layer: PainterLayer);
    fn fl_painter_draw_line_impl(
        data: *const core::ffi::c_void,
        p1: Vec2,
        p2: Vec2,
        color: Color,
        thickness: f32,
    );
    fn fl_painter_draw_rect_impl(
        data: *const core::ffi::c_void,
        p1: Vec2,
        p2: Vec2,
        color: Color,
        rounding: f32,
    );
    fn fl_painter_draw_rect_filled_impl(
        data: *const core::ffi::c_void,
        p1: Vec2,
        p2: Vec2,
        color: Color,
        rounding: f32,
    );
    fn fl_painter_draw_rect_filled_gradient_impl(
        data: *const core::ffi::c_void,
        p1: Vec2,
        p2: Vec2,
        left: Color,
        right: Color,
        btm_right: Color,
        btm_left: Color,
    );
}

#[no_mangle]
pub static mut g_flowi_painter_api: *const PainterFfiApi = std::ptr::null_mut();

#[repr(C)]
#[derive(Debug)]
pub enum PainterLayer {
    ActiveWindow = 0,
    Background = 1,
    Foreground = 2,
}

#[repr(C)]
#[derive(Debug)]
pub struct Painter {
    _dummy: u32,
}

impl Painter {
    /// The current layer to draw on. Default is ActiveWindow.
    pub fn set_layer(layer: PainterLayer) {
        unsafe {
            let _api = &*g_flowi_painter_api;
            #[cfg(any(feature = "static", feature = "tundra"))]
            fl_painter_set_layer_impl(_api.data, layer);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.set_layer)(_api.data, layer);
        }
    }

    /// Draw a line from `pos` to `end` with the given `color` and `thickness`.
    pub fn draw_line(p1: Vec2, p2: Vec2, color: Color, thickness: f32) {
        unsafe {
            let _api = &*g_flowi_painter_api;
            #[cfg(any(feature = "static", feature = "tundra"))]
            fl_painter_draw_line_impl(_api.data, p1, p2, color, thickness);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.draw_line)(_api.data, p1, p2, color, thickness);
        }
    }

    /// Draw a rectangle with the given `color` and `rounding`.
    pub fn draw_rect(p1: Vec2, p2: Vec2, color: Color, rounding: f32) {
        unsafe {
            let _api = &*g_flowi_painter_api;
            #[cfg(any(feature = "static", feature = "tundra"))]
            fl_painter_draw_rect_impl(_api.data, p1, p2, color, rounding);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.draw_rect)(_api.data, p1, p2, color, rounding);
        }
    }

    /// Draw a filled rectangle with the given `color` and `rounding`.
    pub fn draw_rect_filled(p1: Vec2, p2: Vec2, color: Color, rounding: f32) {
        unsafe {
            let _api = &*g_flowi_painter_api;
            #[cfg(any(feature = "static", feature = "tundra"))]
            fl_painter_draw_rect_filled_impl(_api.data, p1, p2, color, rounding);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.draw_rect_filled)(_api.data, p1, p2, color, rounding);
        }
    }

    /// Draw a rectangle with a gradient
    pub fn draw_rect_filled_gradient(
        p1: Vec2,
        p2: Vec2,
        left: Color,
        right: Color,
        btm_right: Color,
        btm_left: Color,
    ) {
        unsafe {
            let _api = &*g_flowi_painter_api;
            #[cfg(any(feature = "static", feature = "tundra"))]
            fl_painter_draw_rect_filled_gradient_impl(
                _api.data, p1, p2, left, right, btm_right, btm_left,
            );
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.draw_rect_filled_gradient)(_api.data, p1, p2, left, right, btm_right, btm_left);
        }
    }
}
