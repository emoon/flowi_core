// This file is auto-generated by api_gen. DO NOT EDIT!

#[allow(unused_imports)]
use crate::manual::{get_last_error, Color, FlString, Result};

#[allow(unused_imports)]
use bitflags::bitflags;

#[allow(unused_imports)]
use crate::math_data::*;

#[repr(C)]
pub struct StyleFfiApi {
    pub(crate) data: *const core::ffi::c_void,
    pub(crate) set_color:
        unsafe extern "C" fn(data: *const core::ffi::c_void, color: StyleColor, value: Color),
    pub(crate) set_color_u32:
        unsafe extern "C" fn(data: *const core::ffi::c_void, color: StyleColor, value: u32),
    pub(crate) push_color_u32:
        unsafe extern "C" fn(data: *const core::ffi::c_void, color: StyleColor, value: u32),
    pub(crate) push_color:
        unsafe extern "C" fn(data: *const core::ffi::c_void, color: StyleColor, value: Color),
    pub(crate) pop_color: unsafe extern "C" fn(data: *const core::ffi::c_void),
    pub(crate) push_single:
        unsafe extern "C" fn(data: *const core::ffi::c_void, style: StyleSingle, value: f32),
    pub(crate) push_vec2:
        unsafe extern "C" fn(data: *const core::ffi::c_void, style: StyleVec2, value: Vec2),
    pub(crate) pop: unsafe extern "C" fn(data: *const core::ffi::c_void),
}

#[cfg(feature = "static")]
extern "C" {
    pub fn fl_style_set_color_impl(data: *const core::ffi::c_void, color: StyleColor, value: Color);
    pub fn fl_style_set_color_u32_impl(
        data: *const core::ffi::c_void,
        color: StyleColor,
        value: u32,
    );
    pub fn fl_style_push_color_u32_impl(
        data: *const core::ffi::c_void,
        color: StyleColor,
        value: u32,
    );
    pub fn fl_style_push_color_impl(
        data: *const core::ffi::c_void,
        color: StyleColor,
        value: Color,
    );
    pub fn fl_style_pop_color_impl(data: *const core::ffi::c_void);
    pub fn fl_style_push_single_impl(
        data: *const core::ffi::c_void,
        style: StyleSingle,
        value: f32,
    );
    pub fn fl_style_push_vec2_impl(data: *const core::ffi::c_void, style: StyleVec2, value: Vec2);
    pub fn fl_style_pop_impl(data: *const core::ffi::c_void);
}

#[no_mangle]
pub static mut g_flowi_style_api: *const StyleFfiApi = std::ptr::null_mut();

#[repr(C)]
#[derive(Debug, Copy, Clone, PartialEq)]
pub enum StyleColor {
    Text = 0,
    TextDisabled = 1,
    WindowBg = 2,
    ChildBg = 3,
    PopupBg = 4,
    Border = 5,
    BorderShadow = 6,
    FrameBg = 7,
    FrameBgHovered = 8,
    FrameBgActive = 9,
    TitleBg = 10,
    TitleBgActive = 11,
    TitleBgCollapsed = 12,
    MenuBarBg = 13,
    ScrollbarBg = 14,
    ScrollbarGrab = 15,
    ScrollbarGrabHovered = 16,
    ScrollbarGrabActive = 17,
    CheckMark = 18,
    SliderGrab = 19,
    SliderGrabActive = 20,
    Button = 21,
    ButtonHovered = 22,
    ButtonActive = 23,
    Header = 24,
    HeaderHovered = 25,
    HeaderActive = 26,
    Separator = 27,
    SeparatorHovered = 28,
    SeparatorActive = 29,
    ResizeGrip = 30,
    ResizeGripHovered = 31,
    ResizeGripActive = 32,
    Tab = 33,
    TabHovered = 34,
    TabActive = 35,
    TabUnfocused = 36,
    TabUnfocusedActive = 37,
    DockingPreview = 38,
    DockingEmptyBg = 39,
    PlotLines = 40,
    PlotLinesHovered = 41,
    PlotHistogram = 42,
    PlotHistogramHovered = 43,
    TableHeaderBg = 44,
    TableBorderStrong = 45,
    TableBorderLight = 46,
    TableRowBg = 47,
    TableRowBgAlt = 48,
    TextSelectedBg = 49,
    DragDropTarget = 50,
    NavHighlight = 51,
    NavWindowingHighlight = 52,
    NavWindowingDimBg = 53,
    ModalWindowDimBg = 54,
}

#[repr(C)]
#[derive(Debug, Copy, Clone, PartialEq)]
pub enum StyleSingle {
    Alpha = 0,
    DisabledAlpha = 1,
    WindowRounding = 2,
    WindowBorderSize = 3,
    ChildRounding = 4,
    ChildBorderSize = 5,
    PopupRounding = 6,
    PopupBorderSize = 7,
    FrameRounding = 8,
    FrameBorderSize = 9,
    IndentSpacing = 10,
    ScrollbarSize = 11,
    ScrollbarRounding = 12,
    GrabMinSize = 13,
    GrabRounding = 14,
    TabRounding = 15,
}

#[repr(C)]
#[derive(Debug, Copy, Clone, PartialEq)]
pub enum StyleVec2 {
    WindowPadding = 0,
    WindowMinSize = 1,
    WindowTitleAlign = 2,
    FramePadding = 3,
    ItemSpacing = 4,
    ItemInnerSpacing = 5,
    CellPadding = 6,
    ButtonTextAlign = 7,
    SelectableTextAlign = 8,
}

#[repr(C)]
#[derive(Debug)]
pub struct Style {
    _dummy: u32,
}

impl Style {
    /// Permantly set a color
    pub fn set_color(color: StyleColor, value: Color) {
        unsafe {
            let _api = &*g_flowi_style_api;
            #[cfg(feature = "static")]
            fl_style_set_color_impl(_api.data, color, value);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.set_color)(_api.data, color, value);
        }
    }

    /// Permantly set a color (ARGB)
    pub fn set_color_u32(color: StyleColor, value: u32) {
        unsafe {
            let _api = &*g_flowi_style_api;
            #[cfg(feature = "static")]
            fl_style_set_color_u32_impl(_api.data, color, value);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.set_color_u32)(_api.data, color, value);
        }
    }

    /// Temporary push a color change (ARGB)
    pub fn push_color_u32(color: StyleColor, value: u32) {
        unsafe {
            let _api = &*g_flowi_style_api;
            #[cfg(feature = "static")]
            fl_style_push_color_u32_impl(_api.data, color, value);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.push_color_u32)(_api.data, color, value);
        }
    }

    /// Temporary push a color change
    pub fn push_color(color: StyleColor, value: Color) {
        unsafe {
            let _api = &*g_flowi_style_api;
            #[cfg(feature = "static")]
            fl_style_push_color_impl(_api.data, color, value);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.push_color)(_api.data, color, value);
        }
    }

    /// Temporary push a color change
    pub fn pop_color() {
        unsafe {
            let _api = &*g_flowi_style_api;
            #[cfg(feature = "static")]
            fl_style_pop_color_impl(_api.data);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.pop_color)(_api.data);
        }
    }

    /// Pushes a single style change
    pub fn push_single(style: StyleSingle, value: f32) {
        unsafe {
            let _api = &*g_flowi_style_api;
            #[cfg(feature = "static")]
            fl_style_push_single_impl(_api.data, style, value);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.push_single)(_api.data, style, value);
        }
    }

    /// Pushes a Vec2 style change
    pub fn push_vec2(style: StyleVec2, value: Vec2) {
        unsafe {
            let _api = &*g_flowi_style_api;
            #[cfg(feature = "static")]
            fl_style_push_vec2_impl(_api.data, style, value);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.push_vec2)(_api.data, style, value);
        }
    }

    /// Pops a style change
    pub fn pop() {
        unsafe {
            let _api = &*g_flowi_style_api;
            #[cfg(feature = "static")]
            fl_style_pop_impl(_api.data);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.pop)(_api.data);
        }
    }
}
