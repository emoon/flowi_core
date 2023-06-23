#pragma once

#include <stdint.h>

struct FlCtx;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum FlKey {
    FlKey_Tab,
    FlKey_LeftArrow,
    FlKey_RightArrow,
    FlKey_UpArrow,
    FlKey_DownArrow,
    FlKey_PageUp,
    FlKey_PageDown,
    FlKey_Home,
    FlKey_End,
    FlKey_Insert,
    FlKey_Delete,
    FlKey_Backspace,
    FlKey_Space,
    FlKey_Enter,
    FlKey_Escape,
    FlKey_LeftCtrl,
    FlKey_LeftShift,
    FlKey_LeftAlt,
    FlKey_LeftSuper,
    FlKey_RightCtrl,
    FlKey_RightShift,
    FlKey_RightAlt,
    FlKey_RightSuper,
    FlKey_Menu,
    FlKey_Alpha0,
    FlKey_Alpha1,
    FlKey_Alpha2,
    FlKey_Alpha3,
    FlKey_Alpha4,
    FlKey_Alpha5,
    FlKey_Alpha6,
    FlKey_Alpha7,
    FlKey_Alpha8,
    FlKey_Alpha9,
    FlKey_A,
    FlKey_B,
    FlKey_C,
    FlKey_D,
    FlKey_E,
    FlKey_F,
    FlKey_G,
    FlKey_H,
    FlKey_I,
    FlKey_J,
    FlKey_K,
    FlKey_L,
    FlKey_M,
    FlKey_N,
    FlKey_O,
    FlKey_P,
    FlKey_Q,
    FlKey_R,
    FlKey_S,
    FlKey_T,
    FlKey_U,
    FlKey_V,
    FlKey_W,
    FlKey_X,
    FlKey_Y,
    FlKey_Z,
    FlKey_F1,
    FlKey_F2,
    FlKey_F3,
    FlKey_F4,
    FlKey_F5,
    FlKey_F6,
    FlKey_F7,
    FlKey_F8,
    FlKey_F9,
    FlKey_F10,
    FlKey_F11,
    FlKey_F12,
    FlKey_Apostrophe,
    FlKey_Comma,
    FlKey_Minus,
    FlKey_Period,
    FlKey_Slash,
    FlKey_Semicolon,
    FlKey_Equal,
    FlKey_LeftBracket,
    FlKey_Backslash,
    FlKey_RightBracket,
    FlKey_GraveAccent,
    FlKey_CapsLock,
    FlKey_ScrollLock,
    FlKey_NumLock,
    FlKey_PrintScreen,
    FlKey_Pause,
    FlKey_Keypad0,
    FlKey_Keypad1,
    FlKey_Keypad2,
    FlKey_Keypad3,
    FlKey_Keypad4,
    FlKey_Keypad5,
    FlKey_Keypad6,
    FlKey_Keypad7,
    FlKey_Keypad8,
    FlKey_Keypad9,
    FlKey_KeypadDecimal,
    FlKey_KeypadDivide,
    FlKey_KeypadMultiply,
    FlKey_KeypadSubtract,
    FlKey_KeypadAdd,
    FlKey_KeypadEnter,
    FlKey_KeypadEqual,
    FlKey_GamepadStart,
    FlKey_GamepadBack,
    FlKey_GamepadFaceLeft,
    FlKey_GamepadFaceRight,
    FlKey_GamepadFaceUp,
    FlKey_GamepadFaceDown,
    FlKey_GamepadDpadLeft,
    FlKey_GamepadDpadRight,
    FlKey_GamepadDpadUp,
    FlKey_GamepadDpadDown,
    FlKey_GamepadL1,
    FlKey_GamepadR1,
    FlKey_GamepadL2,
    FlKey_GamepadR2,
    FlKey_GamepadL3,
    FlKey_GamepadR3,
    FlKey_GamepadLStickLeft,
    FlKey_GamepadLStickRight,
    FlKey_GamepadLStickUp,
    FlKey_GamepadLStickDown,
    FlKey_GamepadRStickLeft,
    FlKey_GamepadRStickRight,
    FlKey_GamepadRStickUp,
    FlKey_GamepadRStickDown,
    FlKey_MouseLeft,
    FlKey_MouseRight,
    FlKey_MouseMiddle,
    FlKey_MouseX1,
    FlKey_MouseX2,
    FlKey_MouseWheelX,
    FlKey_MouseWheelY,
    FlKey_ReservedForModCtrl,
    FlKey_ReservedForModShift,
    FlKey_ReservedForModAlt,
    FlKey_ReservedForModSuper,
    FlKey_ModCtrl,
    FlKey_ModShift,
    FlKey_ModAlt,
    FlKey_ModSuper,
    FlKey_ModShortcut,
} FlKey;

typedef enum FlMouseSource {
    // Input is coming from an actual mouse.
    FlMouseSource_Mouse = 0,         
    // Input is coming from a touch screen 
    // (no hovering prior to initial press, less precise initial press aiming, dual-axis wheeling possible).
    FluiMouseSource_TouchScreen,       
    // Input is coming from a pressure/magnetic pen (often used in conjunction with high-sampling rates).
    FlMouseSource_Pen,               
} FlMouseSource;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FlCtx* fl_ctx_create();
void fl_ctx_destroy(FlCtx* ctx);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The application is responsibe for calling these functions to update the input state.
// This has to be done before fl_ctx_new_frame() is called.

// Queue a new key down/up event. 
// Key should be "translated" (as in, generally ImGuiKey_A matches the key end-user would use to emit an 'A' character)
void fl_input_add_key(struct FlCtx* ctx, FlKey key);

// Queue a new key down/up event for analog values (
// e.g. ImGuiKey_Gamepad_ values). Dead-zones should be handled by the backend.
void fl_input_add_key_analog_event(struct FlCtx* ctx, FlKey key, bool down, float value);

// Queue a mouse position update. Use -FLT_MAX,-FLT_MAX to signify no mouse (e.g. app not focused and not hovered)
void fl_input_add_mouse_pos_event(struct FlCtx* ctx, float x, float y);

// Queue a mouse button change
void fl_input_add_mouse_button_event(struct FlCtx* ctx, int button, bool down);

// Queue a mouse wheel update. 
// wheel_y<0: scroll down, wheel_y>0: scroll up, wheel_x<0: scroll right, wheel_x>0: scroll left.
void fl_input_add_mouse_wheel_event(struct FlCtx* ctx, float x, float y);

// Queue a mouse source change (Mouse/TouchScreen/Pen)
void fl_input_add_mouse_source_event(struct FlCtx* ctx, FlMouseSource source);
    
// Queue a gain/loss of focus for the application (generally based on OS/platform focus of your window)
void fl_input_add_app_focus_event(struct FlCtx* ctx, bool focused);

// Queue a new character input
void fl_input_add_char_event(struct FlCtx* ctx, unsigned int c);

