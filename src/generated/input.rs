// This file is auto-generated by api_gen. DO NOT EDIT!

#[allow(unused_imports)]
use crate::manual::{get_last_error, Color, FlString, Result};

#[allow(unused_imports)]
use bitflags::bitflags;

#[repr(C)]
pub struct InputFfiApi {
    pub(crate) data: *const core::ffi::c_void,
    pub(crate) add_key: unsafe extern "C" fn(data: *const core::ffi::c_void, key: Key),
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
    pub(crate) app_focus_event: unsafe extern "C" fn(data: *const core::ffi::c_void, focused: bool),
    pub(crate) add_char_event: unsafe extern "C" fn(data: *const core::ffi::c_void, c: int),
}

#[cfg(any(feature = "static", feature = "tundra"))]
extern "C" {
    fn fl_input_add_key_impl(data: *const core::ffi::c_void, key: Key);
    fn fl_input_add_key_analog_event_impl(
        data: *const core::ffi::c_void,
        key: Key,
        down: bool,
        value: f32,
    );
    fn fl_input_add_mouse_pos_event_impl(data: *const core::ffi::c_void, x: f32, y: f32);
    fn fl_input_add_mouse_button_event_impl(
        data: *const core::ffi::c_void,
        button: i32,
        down: bool,
    );
    fn fl_input_add_mouse_wheel_event_impl(data: *const core::ffi::c_void, x: f32, y: f32);
    fn fl_input_add_mouse_source_event_impl(data: *const core::ffi::c_void, source: MouseSource);
    fn fl_input_app_focus_event_impl(data: *const core::ffi::c_void, focused: bool);
    fn fl_input_add_char_event_impl(data: *const core::ffi::c_void, c: int);
}

#[no_mangle]
pub static mut g_flowi_input_api: *const InputFfiApi = std::ptr::null_mut();

#[repr(C)]
#[derive(Debug)]
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
    Alpha0 = 24,
    Alpha1 = 25,
    Alpha2 = 26,
    Alpha3 = 27,
    Alpha4 = 28,
    Alpha5 = 29,
    Alpha6 = 30,
    Alpha7 = 31,
    Alpha8 = 32,
    Alpha9 = 33,
    A = 34,
    B = 35,
    C = 36,
    D = 37,
    E = 38,
    F = 39,
    G = 40,
    H = 41,
    I = 42,
    J = 43,
    K = 44,
    L = 45,
    M = 46,
    N = 47,
    O = 48,
    P = 49,
    Q = 50,
    R = 51,
    S = 52,
    T = 53,
    U = 54,
    V = 55,
    W = 56,
    X = 57,
    Y = 58,
    Z = 59,
    F1 = 60,
    F2 = 61,
    F3 = 62,
    F4 = 63,
    F5 = 64,
    F6 = 65,
    F7 = 66,
    F8 = 67,
    F9 = 68,
    F10 = 69,
    F11 = 70,
    F12 = 71,
    Apostrophe = 72,
    Comma = 73,
    Minus = 74,
    Period = 75,
    Slash = 76,
    Semicolon = 77,
    Equal = 78,
    LeftBracket = 79,
    Backslash = 80,
    RightBracket = 81,
    GraveAccent = 82,
    CapsLock = 83,
    ScrollLock = 84,
    NumLock = 85,
    PrintScreen = 86,
    Pause = 87,
    Keypad0 = 88,
    Keypad1 = 89,
    Keypad2 = 90,
    Keypad3 = 91,
    Keypad4 = 92,
    Keypad5 = 93,
    Keypad6 = 94,
    Keypad7 = 95,
    Keypad8 = 96,
    Keypad9 = 97,
    KeypadDecimal = 98,
    KeypadDivide = 99,
    KeypadMultiply = 100,
    KeypadSubtract = 101,
    KeypadAdd = 102,
    KeypadEnter = 103,
    KeypadEqual = 104,
    GamepadStart = 105,
    GamepadBack = 106,
    GamepadFaceLeft = 107,
    GamepadFaceRight = 108,
    GamepadFaceUp = 109,
    GamepadFaceDown = 110,
    GamepadDpadLeft = 111,
    GamepadDpadRight = 112,
    GamepadDpadUp = 113,
    GamepadDpadDown = 114,
    GamepadL1 = 115,
    GamepadR1 = 116,
    GamepadL2 = 117,
    GamepadR2 = 118,
    GamepadL3 = 119,
    GamepadR3 = 120,
    GamepadLStickLeft = 121,
    GamepadLStickRight = 122,
    GamepadLStickUp = 123,
    GamepadLStickDown = 124,
    GamepadRStickLeft = 125,
    GamepadRStickRight = 126,
    GamepadRStickUp = 127,
    GamepadRStickDown = 128,
    MouseLeft = 129,
    MouseRight = 130,
    MouseMiddle = 131,
    MouseX1 = 132,
    MouseX2 = 133,
    MouseWheelX = 134,
    MouseWheelY = 135,
    ReservedForModCtrl = 136,
    ReservedForModShift = 137,
    ReservedForModAlt = 138,
    ReservedForModSuper = 139,
    ModCtrl = 140,
    ModShift = 141,
    ModAlt = 142,
    ModSuper = 143,
    ModShortcut = 144,
}

#[repr(C)]
#[derive(Debug)]
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
#[repr(C)]
#[derive(Debug)]
pub struct Input {
    _dummy: u32,
}

impl Input {
    /// Queue a new key down/up event.
    /// Key should be "translated" (as in, generally [Key::A] matches the key end-user would use to emit an 'A' character)
    pub fn add_key(key: Key) {
        unsafe {
            let _api = &*g_flowi_input_api;
            #[cfg(any(feature = "static", feature = "tundra"))]
            fl_input_add_key_impl(_api.data, key);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.add_key)(_api.data, key);
        }
    }

    /// Queue a new key down/up event for analog values (
    /// e.g. ImGuiKey_Gamepad_ values). Dead-zones should be handled by the backend.
    pub fn add_key_analog_event(key: Key, down: bool, value: f32) {
        unsafe {
            let _api = &*g_flowi_input_api;
            #[cfg(any(feature = "static", feature = "tundra"))]
            fl_input_add_key_analog_event_impl(_api.data, key, down, value);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.add_key_analog_event)(_api.data, key, down, value);
        }
    }

    /// Queue a mouse position update. Use -FLT_MAX,-FLT_MAX to signify no mouse (e.g. app not focused and not hovered)
    pub fn add_mouse_pos_event(x: f32, y: f32) {
        unsafe {
            let _api = &*g_flowi_input_api;
            #[cfg(any(feature = "static", feature = "tundra"))]
            fl_input_add_mouse_pos_event_impl(_api.data, x, y);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.add_mouse_pos_event)(_api.data, x, y);
        }
    }

    /// Queue a mouse button change
    pub fn add_mouse_button_event(button: i32, down: bool) {
        unsafe {
            let _api = &*g_flowi_input_api;
            #[cfg(any(feature = "static", feature = "tundra"))]
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
            #[cfg(any(feature = "static", feature = "tundra"))]
            fl_input_add_mouse_wheel_event_impl(_api.data, x, y);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.add_mouse_wheel_event)(_api.data, x, y);
        }
    }

    /// Queue a mouse source change (Mouse/TouchScreen/Pen)
    pub fn add_mouse_source_event(source: MouseSource) {
        unsafe {
            let _api = &*g_flowi_input_api;
            #[cfg(any(feature = "static", feature = "tundra"))]
            fl_input_add_mouse_source_event_impl(_api.data, source);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.add_mouse_source_event)(_api.data, source);
        }
    }

    /// Queue a gain/loss of focus for the application (generally based on OS/platform focus of your window)
    pub fn app_focus_event(focused: bool) {
        unsafe {
            let _api = &*g_flowi_input_api;
            #[cfg(any(feature = "static", feature = "tundra"))]
            fl_input_app_focus_event_impl(_api.data, focused);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.app_focus_event)(_api.data, focused);
        }
    }

    /// Queue a new character input
    pub fn add_char_event(c: int) {
        unsafe {
            let _api = &*g_flowi_input_api;
            #[cfg(any(feature = "static", feature = "tundra"))]
            fl_input_add_char_event_impl(_api.data, c);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.add_char_event)(_api.data, c);
        }
    }
}
