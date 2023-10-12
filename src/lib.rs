use core::ffi::c_void;
use fileorama::Fileorama;

mod image_loader;
pub mod generated;
pub use generated::*;
mod manual;

pub mod imgui;
pub use manual::Result as Result;

pub struct FlowiState {
    c_data: *const c_void,
    pub vfs: Fileorama,
}

extern "C" {
    fn c_create(settings: *const ApplicationSettings) -> *const c_void;
    fn c_pre_update(data: *const c_void);
    fn c_post_update(data: *const c_void);
}

impl FlowiState {
    pub fn new(settings: &ApplicationSettings, vfs_thread_count: usize) -> Self {
        Self {
            vfs: Fileorama::new(vfs_thread_count),
            c_data: unsafe { c_create(settings) },
        }
    }

    pub fn pre_update(&self) {
        unsafe { c_pre_update(self.c_data) }
    }

    pub fn post_update(&self) {
        unsafe { c_post_update(self.c_data) }
    }
}




