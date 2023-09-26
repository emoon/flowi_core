// This file is auto-generated by api_gen. DO NOT EDIT!

#[allow(unused_imports)]
use crate::manual::{get_last_error, Color, FlString, Result};

#[allow(unused_imports)]
use bitflags::bitflags;

#[repr(C)]
pub struct MenuFfiApi {
    pub(crate) data: *const core::ffi::c_void,
    pub(crate) begin_bar: unsafe extern "C" fn(data: *const core::ffi::c_void) -> bool,
    pub(crate) end_bar: unsafe extern "C" fn(data: *const core::ffi::c_void),
    pub(crate) begin_main_bar: unsafe extern "C" fn(data: *const core::ffi::c_void) -> bool,
    pub(crate) end_main_bar: unsafe extern "C" fn(data: *const core::ffi::c_void),
    pub(crate) begin: unsafe extern "C" fn(
        data: *const core::ffi::c_void,
        label: FlString,
        enabled: bool,
    ) -> bool,
    pub(crate) end: unsafe extern "C" fn(data: *const core::ffi::c_void),
    pub(crate) item: unsafe extern "C" fn(data: *const core::ffi::c_void, label: FlString) -> bool,
    pub(crate) item_ex: unsafe extern "C" fn(
        data: *const core::ffi::c_void,
        label: FlString,
        shortcut: FlString,
        selected: bool,
        enabled: bool,
    ) -> bool,
    pub(crate) item_toggle: unsafe extern "C" fn(
        data: *const core::ffi::c_void,
        label: FlString,
        shortcut: FlString,
        selected: *mut bool,
        enabled: bool,
    ) -> bool,
}

#[cfg(feature = "static")]
extern "C" {
    pub fn fl_menu_begin_bar_impl(data: *const core::ffi::c_void) -> bool;
    pub fn fl_menu_end_bar_impl(data: *const core::ffi::c_void);
    pub fn fl_menu_begin_main_bar_impl(data: *const core::ffi::c_void) -> bool;
    pub fn fl_menu_end_main_bar_impl(data: *const core::ffi::c_void);
    pub fn fl_menu_begin_impl(
        data: *const core::ffi::c_void,
        label: FlString,
        enabled: bool,
    ) -> bool;
    pub fn fl_menu_end_impl(data: *const core::ffi::c_void);
    pub fn fl_menu_item_impl(data: *const core::ffi::c_void, label: FlString) -> bool;
    pub fn fl_menu_item_ex_impl(
        data: *const core::ffi::c_void,
        label: FlString,
        shortcut: FlString,
        selected: bool,
        enabled: bool,
    ) -> bool;
    pub fn fl_menu_item_toggle_impl(
        data: *const core::ffi::c_void,
        label: FlString,
        shortcut: FlString,
        selected: *mut bool,
        enabled: bool,
    ) -> bool;
}

#[no_mangle]
pub static mut g_flowi_menu_api: *const MenuFfiApi = std::ptr::null_mut();

#[repr(C)]
#[derive(Debug)]
pub struct Menu {
    _dummy: u32,
}

impl Menu {
    /// Append to menu-bar of current window (requires [WindowFlags::MENU_BAR] flag set on parent window).
    pub fn begin_bar() -> bool {
        unsafe {
            let _api = &*g_flowi_menu_api;
            #[cfg(feature = "static")]
            let ret_val = fl_menu_begin_bar_impl(_api.data);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            let ret_val = (_api.begin_bar)(_api.data);
            ret_val
        }
    }

    /// only call end_bar() if begin_bar() returns true!
    pub fn end_bar() {
        unsafe {
            let _api = &*g_flowi_menu_api;
            #[cfg(feature = "static")]
            fl_menu_end_bar_impl(_api.data);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.end_bar)(_api.data);
        }
    }

    /// create and append to a full screen menu-bar.
    pub fn begin_main_bar() -> bool {
        unsafe {
            let _api = &*g_flowi_menu_api;
            #[cfg(feature = "static")]
            let ret_val = fl_menu_begin_main_bar_impl(_api.data);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            let ret_val = (_api.begin_main_bar)(_api.data);
            ret_val
        }
    }

    /// only call end_main_bar() if begin_main_bar() returns true!
    pub fn end_main_bar() {
        unsafe {
            let _api = &*g_flowi_menu_api;
            #[cfg(feature = "static")]
            fl_menu_end_main_bar_impl(_api.data);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.end_main_bar)(_api.data);
        }
    }

    /// create a sub-menu entry. only call EndMenu() if this returns true!
    pub fn begin(label: &str, enabled: bool) -> bool {
        unsafe {
            let _api = &*g_flowi_menu_api;
            #[cfg(feature = "static")]
            let ret_val = fl_menu_begin_impl(_api.data, FlString::new(label), enabled);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            let ret_val = (_api.begin)(_api.data, FlString::new(label), enabled);
            ret_val
        }
    }

    /// only call end_menu() if begin_menu() returns true!
    pub fn end() {
        unsafe {
            let _api = &*g_flowi_menu_api;
            #[cfg(feature = "static")]
            fl_menu_end_impl(_api.data);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.end)(_api.data);
        }
    }

    /// return true when activated.
    pub fn item(label: &str) -> bool {
        unsafe {
            let _api = &*g_flowi_menu_api;
            #[cfg(feature = "static")]
            let ret_val = fl_menu_item_impl(_api.data, FlString::new(label));
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            let ret_val = (_api.item)(_api.data, FlString::new(label));
            ret_val
        }
    }

    /// return true when activated. Includes some extra info such as shortcut, etc
    pub fn item_ex(label: &str, shortcut: &str, selected: bool, enabled: bool) -> bool {
        unsafe {
            let _api = &*g_flowi_menu_api;
            #[cfg(feature = "static")]
            let ret_val = fl_menu_item_ex_impl(
                _api.data,
                FlString::new(label),
                FlString::new(shortcut),
                selected,
                enabled,
            );
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            let ret_val = (_api.item_ex)(
                _api.data,
                FlString::new(label),
                FlString::new(shortcut),
                selected,
                enabled,
            );
            ret_val
        }
    }

    /// return true when activated + toggle selected
    pub fn item_toggle(label: &str, shortcut: &str, selected: &mut bool, enabled: bool) -> bool {
        unsafe {
            let _api = &*g_flowi_menu_api;
            #[cfg(feature = "static")]
            let ret_val = fl_menu_item_toggle_impl(
                _api.data,
                FlString::new(label),
                FlString::new(shortcut),
                selected as _,
                enabled,
            );
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            let ret_val = (_api.item_toggle)(
                _api.data,
                FlString::new(label),
                FlString::new(shortcut),
                selected as _,
                enabled,
            );
            ret_val
        }
    }
}
