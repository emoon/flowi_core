use core::ffi::c_void;
use fileorama::Fileorama;
use image_api::ImageHandler;

pub mod generated;
pub mod image_api;
pub use generated::*;
mod manual;

pub mod imgui;
pub use manual::Result;

#[repr(C)]
pub(crate) struct InternalState {
    pub(crate) vfs: Fileorama,
    pub(crate) image_handler: ImageHandler,
}

pub struct FlowiState {
    c_data: *const c_void,
    state: Box<InternalState>,
}

extern "C" {
    fn c_create(
        settings: *const ApplicationSettings,
        rust_state: *const InternalState,
    ) -> *const c_void;
    fn c_pre_update(data: *const c_void);
    fn c_post_update(data: *const c_void);
}

impl FlowiState {
    pub fn new(settings: &ApplicationSettings, vfs_thread_count: usize) -> Self {
        let state = Box::new(InternalState {
            vfs: Fileorama::new(vfs_thread_count),
            image_handler: ImageHandler::new(),
        });

        let ptr = Box::into_raw(state);
        let c_data = unsafe { c_create(settings, ptr) };
        let state = unsafe { Box::from_raw(ptr) };

        Self { c_data, state }
    }

    pub fn pre_update(&self) {
        unsafe { c_pre_update(self.c_data) }
    }

    pub fn post_update(&self) {
        unsafe { c_post_update(self.c_data) }
    }
}
