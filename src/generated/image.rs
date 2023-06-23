// This file is auto-generated by api_gen. DO NOT EDIT!

#[allow(unused_imports)]
use crate::manual::{get_last_error, Color, FlString, Result};

#[allow(unused_imports)]
use bitflags::bitflags;

#[repr(C)]
pub struct ImageFfiApi {
    pub(crate) data: *const core::ffi::c_void,
    pub(crate) get_info:
        unsafe extern "C" fn(data: *const core::ffi::c_void, image: u64) -> *const ImageInfo,
}

#[cfg(any(feature = "static", feature = "tundra"))]
extern "C" {
    fn fl_image_get_info_impl(data: *const core::ffi::c_void, image: u64) -> *const ImageInfo;
}

#[no_mangle]
pub static mut g_flowi_image_api: *const ImageFfiApi = std::ptr::null_mut();

#[repr(C)]
#[derive(Debug)]
pub enum SvgFlags {
    /// Render the SVG image using RGBA format
    Rgba = 0,
    /// Render the SVG image using Alpha only
    Alpha = 1,
}

#[repr(C)]
#[derive(Debug)]
pub struct ImageInfo {
    /// width of the image
    pub width: u32,
    /// height of the Image
    pub height: u32,
}

#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct Image {
    pub handle: u64,
}

impl Image {
    /// Load image from file. Supported formats are:
    /// JPEG baseline & progressive (12 bpc/arithmetic not supported, same as stock IJG lib)
    /// PNG 1/2/4/8/16-bit-per-channel
    /// TGA
    /// BMP non-1bpp, non-RLE
    /// PSD (composited view only, no extra channels, 8/16 bit-per-channel)
    /// GIF
    /// HDR (radiance rgbE format)
    /// PIC (Softimage PIC)
    /// PNM (PPM and PGM binary only)
    /// Load image from memory. Supported formats are:
    /// JPEG baseline & progressive (12 bpc/arithmetic not supported, same as stock IJG lib)
    /// PNG 1/2/4/8/16-bit-per-channel
    /// TGA
    /// BMP non-1bpp, non-RLE
    /// PSD (composited view only, no extra channels, 8/16 bit-per-channel)
    /// GIF
    /// HDR (radiance rgbE format)
    /// PIC (Softimage PIC)
    /// PNM (PPM and PGM binary only)
    /// Load SVG from file
    /// Load SVG from memory
    /// Get data amout the image
    pub fn get_info<'a>(image: Image) -> Result<&'a ImageInfo> {
        unsafe {
            let _api = &*g_flowi_image_api;
            #[cfg(any(feature = "static", feature = "tundra"))]
            let ret_val = fl_image_get_info_impl(_api.data, image.handle);
            #[cfg(any(feature = "dynamic", feature = "plugin"))]
            let ret_val = (_api.get_info)(_api.data, image.handle);
            if ret_val.is_null() {
                Err(get_last_error())
            } else {
                Ok(&*ret_val)
            }
        }
    }
}
