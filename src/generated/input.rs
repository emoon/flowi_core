// This file is auto-generated by api_gen. DO NOT EDIT!

#[allow(unused_imports)]
use crate::manual::{get_last_error, Color, FlString, Result};

#[allow(unused_imports)]
use bitflags::bitflags;

#[repr(C)]
pub struct InputFfiApi {
    pub(crate) data: *const core::ffi::c_void,
    pub(crate) add_key_event:
        unsafe extern "C" fn(data: *const core::ffi::c_void, key: Key, down: bool),
    pub(crate) add_key_analog_event:
        unsafe extern "C" fn(data: *const core::ffi::c_void, key: Key, down: bool, value: f32),
    pub(crate) add_mouse_pos_event:
        unsafe extern "C" fn(data: *const core::ffi::c_void, x: f32, y: f32),
    pub(crate) add_mouse_button_event:
        unsafe extern "C" fn(data: *const core::ffi::c_void, button: i32, down: bool),
    pub(crate) add_mouse_wheel_event:
        unsafe extern "C" fn(data: *const core::ffi::c_void, x: f32, y: f32),
    pub(crate) add_mouse_source_event:
        unsafe extern "C" fn(data: *const core::ffi::c_void, source: MouseSource),
    pub(crate) add_focus_event: unsafe extern "C" fn(data: *const core::ffi::c_void, focused: bool),
    pub(crate) add_char_event: unsafe extern "C" fn(data: *const core::ffi::c_void, c: i32),
    pub(crate) update_screen_size_time: unsafe extern "C" fn(
        data: *const core::ffi::c_void,
        display_h: f32,
        display_w: f32,
        w: f32,
        h: f32,
        delta_time: f32,
    ),
}

#[cfg(feature = "static")]
extern "C" {
    pub fn fl_input_add_key_event_impl(data: *const core::ffi::c_void, key: Key, down: bool);
    pub fn fl_input_add_key_analog_event_impl(
        data: *const core::ffi::c_void,
        key: Key,
        down: bool,
        value: f32,
    );
    pub fn fl_input_add_mouse_pos_event_impl(data: *const core::ffi::c_void, x: f32, y: f32);
    pub fn fl_input_add_mouse_button_event_impl(
        data: *const core::ffi::c_void,
        button: i32,
        down: bool,
    );
    pub fn fl_input_add_mouse_wheel_event_impl(data: *const core::ffi::c_void, x: f32, y: f32);
    pub fn fl_input_add_mouse_source_event_impl(
        data: *const core::ffi::c_void,
        source: MouseSource,
    );
    pub fn fl_input_add_focus_event_impl(data: *const core::ffi::c_void, focused: bool);
    pub fn fl_input_add_char_event_impl(data: *const core::ffi::c_void, c: i32);
    pub fn fl_input_update_screen_size_time_impl(
        data: *const core::ffi::c_void,
        display_h: f32,
        display_w: f32,
        w: f32,
        h: f32,
        delta_time: f32,
    );
}

#[no_mangle]
pub static mut g_flowi_input_api: *const InputFfiApi = std::ptr::null_mut();

#[repr(C)]
#[derive(Debug, Copy, Clone, PartialEq)]
pub enum Key {
    Tab = 0,
    LeftArrow = 1,
    RightArrow = 2,
    UpArrow = 3,
    DownArrow = 4,
    PageUp = 5,
    PageDown = 6,
    Home = 7,
    End = 8,
    Insert = 9,
    Delete = 10,
    Backspace = 11,
    Space = 12,
    Enter = 13,
    Escape = 14,
    LeftCtrl = 15,
    LeftShift = 16,
    LeftAlt = 17,
    LeftSuper = 18,
    RightCtrl = 19,
    RightShift = 20,
    RightAlt = 21,
    RightSuper = 22,
    Menu = 23,
    A = 24,
    B = 25,
    C = 26,
    D = 27,
    E = 28,
    F = 29,
    G = 30,
    H = 31,
    I = 32,
    J = 33,
    K = 34,
    L = 35,
    M = 36,
    N = 37,
    O = 38,
    P = 39,
    Q = 40,
    R = 41,
    S = 42,
    T = 43,
    U = 44,
    V = 45,
    W = 46,
    X = 47,
    Y = 48,
    Z = 49,
    F1 = 50,
    F2 = 51,
    F3 = 52,
    F4 = 53,
    F5 = 54,
    F6 = 55,
    F7 = 56,
    F8 = 57,
    F9 = 58,
    F10 = 59,
    F11 = 60,
    F12 = 61,
    Apostrophe = 62,
    Comma = 63,
    Minus = 64,
    Period = 65,
    Slash = 66,
    Semicolon = 67,
    Equal = 68,
    LeftBracket = 69,
    Backslash = 70,
    RightBracket = 71,
    GraveAccent = 72,
    CapsLock = 73,
    ScrollLock = 74,
    NumLock = 75,
    PrintScreen = 76,
    Pause = 77,
    Keypad0 = 78,
    Keypad1 = 79,
    Keypad2 = 80,
    Keypad3 = 81,
    Keypad4 = 82,
    Keypad5 = 83,
    Keypad6 = 84,
    Keypad7 = 85,
    Keypad8 = 86,
    Keypad9 = 87,
    KeypadDecimal = 88,
    KeypadDivide = 89,
    KeypadMultiply = 90,
    KeypadSubtract = 91,
    KeypadAdd = 92,
    KeypadEnter = 93,
    KeypadEqual = 94,
    GamepadStart = 95,
    GamepadBack = 96,
    GamepadFaceLeft = 97,
    GamepadFaceRight = 98,
    GamepadFaceUp = 99,
    GamepadFaceDown = 100,
    GamepadDpadLeft = 101,
    GamepadDpadRight = 102,
    GamepadDpadUp = 103,
    GamepadDpadDown = 104,
    GamepadL1 = 105,
    GamepadR1 = 106,
    GamepadL2 = 107,
    GamepadR2 = 108,
    GamepadL3 = 109,
    GamepadR3 = 110,
    GamepadLStickLeft = 111,
    GamepadLStickRight = 112,
    GamepadLStickUp = 113,
    GamepadLStickDown = 114,
    GamepadRStickLeft = 115,
    GamepadRStickRight = 116,
    GamepadRStickUp = 117,
    GamepadRStickDown = 118,
    MouseLeft = 119,
    MouseRight = 120,
    MouseMiddle = 121,
    MouseX1 = 122,
    MouseX2 = 123,
    MouseWheelX = 124,
    MouseWheelY = 125,
    ModCtrl = 126,
    ModShift = 127,
    ModAlt = 128,
    ModSuper = 129,
    ModShortcut = 130,
}

#[repr(C)]
#[derive(Debug, Copy, Clone, PartialEq)]
pub enum MouseSource {
    /// Input is coming from an actual mouse.
    Mouse = 0,
    /// Input is coming from a touch screen
    /// (no hovering prior to initial press, less precise initial press aiming, dual-axis wheeling possible).
    TouchScreen = 1,
    /// Input is coming from a pressure/magnetic pen (often used in conjunction with high-sampling rates).
    Pen = 2,
}

/// The application is responsibe for calling these functions to update the input state.
/// This has to be done before fl_ctx_new_frame() is called.
#[repr(C)]
#[derive(Debug)]
pub struct Input {
    _dummy: u32,
}

impl Input {
    /// Queue a new key down/up event.
    /// Key should be "translated" (as in, generally [Key::A] matches the key end-user would use to emit an 'A' character)
    pub fn add_key_event(key: Key, down: bool) {
        unsafe {
            let _api = &*g_flowi_input_api;
            #[cfg(feature = "static")]
            fl_input_add_key_event_impl(_api.data, key, down);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.add_key_event)(_api.data, key, down);
        }
    }

    /// Queue a new key down/up event for analog values (
    /// e.g. ImGuiKey_Gamepad_ values). Dead-zones should be handled by the backend.
    pub fn add_key_analog_event(key: Key, down: bool, value: f32) {
        unsafe {
            let _api = &*g_flowi_input_api;
            #[cfg(feature = "static")]
            fl_input_add_key_analog_event_impl(_api.data, key, down, value);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.add_key_analog_event)(_api.data, key, down, value);
        }
    }

    /// Queue a mouse position update. Use -FLT_MAX,-FLT_MAX to signify no mouse (e.g. app not focused and not hovered)
    pub fn add_mouse_pos_event(x: f32, y: f32) {
        unsafe {
            let _api = &*g_flowi_input_api;
            #[cfg(feature = "static")]
            fl_input_add_mouse_pos_event_impl(_api.data, x, y);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.add_mouse_pos_event)(_api.data, x, y);
        }
    }

    /// Queue a mouse button change
    pub fn add_mouse_button_event(button: i32, down: bool) {
        unsafe {
            let _api = &*g_flowi_input_api;
            #[cfg(feature = "static")]
            fl_input_add_mouse_button_event_impl(_api.data, button, down);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.add_mouse_button_event)(_api.data, button, down);
        }
    }

    /// Queue a mouse wheel update.
    /// wheel_y<0: scroll down, wheel_y>0: scroll up, wheel_x<0: scroll right, wheel_x>0: scroll left.
    pub fn add_mouse_wheel_event(x: f32, y: f32) {
        unsafe {
            let _api = &*g_flowi_input_api;
            #[cfg(feature = "static")]
            fl_input_add_mouse_wheel_event_impl(_api.data, x, y);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.add_mouse_wheel_event)(_api.data, x, y);
        }
    }

    /// Queue a mouse source change (Mouse/TouchScreen/Pen)
    pub fn add_mouse_source_event(source: MouseSource) {
        unsafe {
            let _api = &*g_flowi_input_api;
            #[cfg(feature = "static")]
            fl_input_add_mouse_source_event_impl(_api.data, source);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.add_mouse_source_event)(_api.data, source);
        }
    }

    /// Queue a gain/loss of focus for the application (generally based on OS/platform focus of your window)
    pub fn add_focus_event(focused: bool) {
        unsafe {
            let _api = &*g_flowi_input_api;
            #[cfg(feature = "static")]
            fl_input_add_focus_event_impl(_api.data, focused);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.add_focus_event)(_api.data, focused);
        }
    }

    /// Queue a new character input
    pub fn add_char_event(c: i32) {
        unsafe {
            let _api = &*g_flowi_input_api;
            #[cfg(feature = "static")]
            fl_input_add_char_event_impl(_api.data, c);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.add_char_event)(_api.data, c);
        }
    }

    /// This is a bit temporary and should be moved
    pub fn update_screen_size_time(
        display_h: f32,
        display_w: f32,
        w: f32,
        h: f32,
        delta_time: f32,
    ) {
        unsafe {
            let _api = &*g_flowi_input_api;
            #[cfg(feature = "static")]
            fl_input_update_screen_size_time_impl(
                _api.data, display_h, display_w, w, h, delta_time,
            );
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.update_screen_size_time)(_api.data, display_h, display_w, w, h, delta_time);
        }
    }
}
