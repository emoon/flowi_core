use core::ffi::c_void;
use std::fmt::Formatter;
use std::fmt::Debug;


#[repr(C)]
pub struct FlString {
    string: *const c_void,
    length: u32,
}

impl FlString {
    pub fn new(s: &str) -> Self {
        FlString {
            string: s.as_ptr() as *const c_void,
            length: s.len() as u32,
        }
    }

    pub fn as_str(&self) -> &str {
        let s =
            unsafe { std::slice::from_raw_parts(self.string as *const u8, self.length as usize) };
        std::str::from_utf8(s).unwrap()
    }
}

#[cfg(any(feature = "dynamic", feature = "static", feature = "tundra"))]
impl Debug for FlString {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        let s =
            unsafe { std::slice::from_raw_parts(self.string as *const u8, self.length as usize) };
        let s = std::str::from_utf8(s).unwrap();
        write!(f, "{}", s)
    }
}

#[derive(Debug)]
pub struct FlowiError {
    pub message: u32,
}

#[repr(C)]
#[derive(Debug, Clone, Copy)]
pub struct Color {
    pub r: f32,
    pub g: f32,
    pub b: f32,
    pub a: f32,
}

pub fn get_last_error() -> FlowiError {
    // TODO: Implement
    FlowiError { message: 0 }
}

pub type Result<T> = core::result::Result<T, FlowiError>;

