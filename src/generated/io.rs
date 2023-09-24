// This file is auto-generated by api_gen. DO NOT EDIT!

#[allow(unused_imports)]
use crate::manual::{get_last_error, Color, FlString, Result};

#[allow(unused_imports)]
use bitflags::bitflags;

#[allow(unused_imports)]
use crate::image::*;

#[allow(unused_imports)]
use crate::shader::*;

#[repr(C)]
pub struct IoFfiApi {
    pub(crate) data: *const core::ffi::c_void,
    pub(crate) load_image:
        unsafe extern "C" fn(data: *const core::ffi::c_void, filename: FlString) -> u64,
    pub(crate) load_image_async:
        unsafe extern "C" fn(data: *const core::ffi::c_void, filename: FlString) -> u64,
    pub(crate) load_file:
        unsafe extern "C" fn(data: *const core::ffi::c_void, filename: FlString) -> u64,
    pub(crate) load_shader_program_comp: unsafe extern "C" fn(
        data: *const core::ffi::c_void,
        vs_filename: FlString,
        ps_filename: FlString,
    ) -> u64,
}

#[cfg(feature = "static")]
extern "C" {
    fn fl_io_load_image_impl(data: *const core::ffi::c_void, filename: FlString) -> u64;
    fn fl_io_load_image_async_impl(data: *const core::ffi::c_void, filename: FlString) -> u64;
    fn fl_io_load_file_impl(data: *const core::ffi::c_void, filename: FlString) -> u64;
    fn fl_io_load_shader_program_comp_impl(
        data: *const core::ffi::c_void,
        vs_filename: FlString,
        ps_filename: FlString,
    ) -> u64;
}

#[no_mangle]
pub static mut g_flowi_io_api: *const IoFfiApi = std::ptr::null_mut();

#[repr(C)]
#[derive(Debug)]
pub struct Io {
    _dummy: u32,
}

impl Io {
    /// Load image from file/url. Supported formats are: JPG, PNG, WEBP
    pub fn load_image(filename: &str) -> Image {
        unsafe {
            let _api = &*g_flowi_io_api;
            #[cfg(feature = "static")]
            let ret_val = fl_io_load_image_impl(_api.data, FlString::new(filename));
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            let ret_val = (_api.load_image)(_api.data, FlString::new(filename));
            Image { handle: ret_val }
        }
    }

    /// Load image async from file/url. Supported formats are: JPG, PNG, WEBP
    pub fn load_image_async(filename: &str) -> Result<Image> {
        unsafe {
            let _api = &*g_flowi_io_api;
            #[cfg(feature = "static")]
            let ret_val = fl_io_load_image_async_impl(_api.data, FlString::new(filename));
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            let ret_val = (_api.load_image_async)(_api.data, FlString::new(filename));
            if ret_val == 0 {
                Err(get_last_error())
            } else {
                Ok(Image { handle: ret_val })
            }
        }
    }

    /// Load image from file/url
    pub fn load_file(filename: &str) -> Result<Image> {
        unsafe {
            let _api = &*g_flowi_io_api;
            #[cfg(feature = "static")]
            let ret_val = fl_io_load_file_impl(_api.data, FlString::new(filename));
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            let ret_val = (_api.load_file)(_api.data, FlString::new(filename));
            if ret_val == 0 {
                Err(get_last_error())
            } else {
                Ok(Image { handle: ret_val })
            }
        }
    }

    /// Same as load_image_from_url, but async and gives back a handle to check/access data later.
    /// Load a vertex shader be used for rendering. This will also compile the shader.
    /// Load a pixel shader to be used for rendering. This will also compile the shader.
    /// Load a vertex shader and pixel shader to be used as a shader program. This will also compile the shaders.
    pub fn load_shader_program_comp(vs_filename: &str, ps_filename: &str) -> Result<ShaderProgram> {
        unsafe {
            let _api = &*g_flowi_io_api;
            #[cfg(feature = "static")]
            let ret_val = fl_io_load_shader_program_comp_impl(
                _api.data,
                FlString::new(vs_filename),
                FlString::new(ps_filename),
            );
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            let ret_val = (_api.load_shader_program_comp)(
                _api.data,
                FlString::new(vs_filename),
                FlString::new(ps_filename),
            );
            if ret_val == 0 {
                Err(get_last_error())
            } else {
                Ok(ShaderProgram { handle: ret_val })
            }
        }
    }
}
