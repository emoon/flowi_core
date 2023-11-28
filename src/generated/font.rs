// This file is auto-generated by api_gen. DO NOT EDIT!

#[allow(unused_imports)]
use crate::manual::{get_last_error, Color, FlString, Result};

#[allow(unused_imports)]
use bitflags::bitflags;

#[repr(C)]
pub struct FontFfiApi {
    pub(crate) data: *const core::ffi::c_void,
    pub(crate) load: unsafe extern "C" fn(
        data: *const core::ffi::c_void,
        filename: FlString,
        font_size: u32,
    ) -> u64,
    pub(crate) load_with_range: unsafe extern "C" fn(
        data: *const core::ffi::c_void,
        filename: FlString,
        font_size: u32,
        glyph_range_start: u16,
        glyph_range_end: u16,
    ) -> u64,
    pub(crate) load_from_memory: unsafe extern "C" fn(
        data: *const core::ffi::c_void,
        name: FlString,
        data: *const u8,
        data_size: u32,
        font_size: u32,
    ) -> u64,
    pub(crate) push: unsafe extern "C" fn(data: *const core::ffi::c_void, font: u64),
    pub(crate) pop: unsafe extern "C" fn(data: *const core::ffi::c_void),
    pub(crate) destroy: unsafe extern "C" fn(data: *const core::ffi::c_void, font: u64),
}

#[cfg(any(feature = "static", feature = "tundra"))]
extern "C" {
    fn fl_font_load_impl(data: *const core::ffi::c_void, filename: FlString, font_size: u32)
        -> u64;
    fn fl_font_load_with_range_impl(
        data: *const core::ffi::c_void,
        filename: FlString,
        font_size: u32,
        glyph_range_start: u16,
        glyph_range_end: u16,
    ) -> u64;
    fn fl_font_load_from_memory_impl(
        data: *const core::ffi::c_void,
        name: FlString,
        data: *const u8,
        data_size: u32,
        font_size: u32,
    ) -> u64;
    fn fl_font_push_impl(data: *const core::ffi::c_void, font: u64);
    fn fl_font_pop_impl(data: *const core::ffi::c_void);
    fn fl_font_destroy_impl(data: *const core::ffi::c_void, font: u64);
}

#[no_mangle]
pub static mut g_flowi_font_api: *const FontFfiApi = std::ptr::null_mut();

#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct Font {
    pub handle: u64,
}

impl Font {
    /// Create a font from (TTF) file. To use the font use [Font::set_font] before using text-based widgets
    /// Returns >= 0 for valid handle, use fl_get_status(); for more detailed error message
    pub fn load(filename: &str, font_size: u32) -> Result<Font> {
        unsafe {
            let _api = &*g_flowi_font_api;
            #[cfg(any(feature = "static", feature = "tundra"))]
            let ret_val = fl_font_load_impl(_api.data, FlString::new(filename), font_size);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            let ret_val = (_api.load)(_api.data, FlString::new(filename), font_size);
            if ret_val == 0 {
                Err(get_last_error())
            } else {
                Ok(Font { handle: ret_val })
            }
        }
    }

    /// Create an new font from a FFT file with a range of characters that should be pre-generated
    pub fn load_with_range(
        filename: &str,
        font_size: u32,
        glyph_range_start: u16,
        glyph_range_end: u16,
    ) -> Result<Font> {
        unsafe {
            let _api = &*g_flowi_font_api;
            #[cfg(any(feature = "static", feature = "tundra"))]
            let ret_val = fl_font_load_with_range_impl(
                _api.data,
                FlString::new(filename),
                font_size,
                glyph_range_start,
                glyph_range_end,
            );
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            let ret_val = (_api.load_with_range)(
                _api.data,
                FlString::new(filename),
                font_size,
                glyph_range_start,
                glyph_range_end,
            );
            if ret_val == 0 {
                Err(get_last_error())
            } else {
                Ok(Font { handle: ret_val })
            }
        }
    }

    /// Create a font from memory. Data is expected to point to a TTF file. Fl will take a copy of this data in some cases
    /// Like when needing the accurate placement mode used by Harzbuff that needs to original ttf data
    pub fn load_from_memory(name: &str, data: &[u8], font_size: u32) -> Result<Font> {
        unsafe {
            let _api = &*g_flowi_font_api;
            #[cfg(any(feature = "static", feature = "tundra"))]
            let ret_val = fl_font_load_from_memory_impl(
                _api.data,
                FlString::new(name),
                data.as_ptr(),
                data.len() as _,
                font_size,
            );
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            let ret_val = (_api.load_from_memory)(
                _api.data,
                FlString::new(name),
                data.as_ptr(),
                data.len() as _,
                font_size,
            );
            if ret_val == 0 {
                Err(get_last_error())
            } else {
                Ok(Font { handle: ret_val })
            }
        }
    }

    /// Push a font for usage
    pub fn push(font: Font) {
        unsafe {
            let _api = &*g_flowi_font_api;
            #[cfg(any(feature = "static", feature = "tundra"))]
            fl_font_push_impl(_api.data, font.handle);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.push)(_api.data, font.handle);
        }
    }

    /// Pop a font from the stack
    pub fn pop() {
        unsafe {
            let _api = &*g_flowi_font_api;
            #[cfg(any(feature = "static", feature = "tundra"))]
            fl_font_pop_impl(_api.data);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.pop)(_api.data);
        }
    }

    /// Destory the current font, render the id invalid
    pub fn destroy(font: Font) {
        unsafe {
            let _api = &*g_flowi_font_api;
            #[cfg(any(feature = "static", feature = "tundra"))]
            fl_font_destroy_impl(_api.data, font.handle);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            (_api.destroy)(_api.data, font.handle);
        }
    }
}
