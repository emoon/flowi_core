// This file is auto-generated by api_gen. DO NOT EDIT!

use core::ffi::c_void;
pub mod application_settings;
pub mod button;
pub mod context;
pub mod debug;
pub mod error;
pub mod font;
pub mod image;
pub mod input;
pub mod io;
pub mod item;
pub mod layout;
pub mod math_data;
pub mod menu;
pub mod painter;
pub mod render_commands;
pub mod shader;
pub mod style;
pub mod text;
pub mod ui;
pub mod window;
use crate::button::ButtonFfiApi;
use crate::font::FontFfiApi;
pub use crate::generated::button::Button;
pub use crate::generated::font::Font;
pub use crate::generated::image::Image;
pub use crate::generated::input::Input;
pub use crate::generated::item::Item;
pub use crate::generated::layout::Cursor;
pub use crate::generated::menu::Menu;
pub use crate::generated::painter::Painter;
pub use crate::generated::style::Style;
pub use crate::generated::text::Text;
pub use crate::generated::ui::Ui;
pub use crate::generated::window::Window;
use crate::image::ImageFfiApi;
use crate::input::InputFfiApi;
use crate::item::ItemFfiApi;
use crate::layout::CursorFfiApi;
use crate::menu::MenuFfiApi;
use crate::painter::PainterFfiApi;
use crate::style::StyleFfiApi;
use crate::text::TextFfiApi;
use crate::ui::UiFfiApi;
use crate::window::WindowFfiApi;

#[repr(C)]
pub(crate) struct AppFfi {
    pub(crate) data: *const c_void,
    pub(crate) main_loop: unsafe fn(data: *const c_void, user_data: *mut c_void) -> bool,
    pub(crate) button_get_api:
        unsafe extern "C" fn(data: *const c_void, api_ver: u32) -> *const ButtonFfiApi,
    pub(crate) cursor_get_api:
        unsafe extern "C" fn(data: *const c_void, api_ver: u32) -> *const CursorFfiApi,
    pub(crate) font_get_api:
        unsafe extern "C" fn(data: *const c_void, api_ver: u32) -> *const FontFfiApi,
    pub(crate) image_get_api:
        unsafe extern "C" fn(data: *const c_void, api_ver: u32) -> *const ImageFfiApi,
    pub(crate) input_get_api:
        unsafe extern "C" fn(data: *const c_void, api_ver: u32) -> *const InputFfiApi,
    pub(crate) item_get_api:
        unsafe extern "C" fn(data: *const c_void, api_ver: u32) -> *const ItemFfiApi,
    pub(crate) menu_get_api:
        unsafe extern "C" fn(data: *const c_void, api_ver: u32) -> *const MenuFfiApi,
    pub(crate) painter_get_api:
        unsafe extern "C" fn(data: *const c_void, api_ver: u32) -> *const PainterFfiApi,
    pub(crate) style_get_api:
        unsafe extern "C" fn(data: *const c_void, api_ver: u32) -> *const StyleFfiApi,
    pub(crate) text_get_api:
        unsafe extern "C" fn(data: *const c_void, api_ver: u32) -> *const TextFfiApi,
    pub(crate) ui_get_api:
        unsafe extern "C" fn(data: *const c_void, api_ver: u32) -> *const UiFfiApi,
    pub(crate) window_get_api:
        unsafe extern "C" fn(data: *const c_void, api_ver: u32) -> *const WindowFfiApi,
}
