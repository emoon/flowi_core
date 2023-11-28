pub mod generated;
pub use generated::*;
pub mod manual;
pub use manual::*;

pub use crate::application_settings::ApplicationSettings;

pub struct Instance {
    pub dummy: u32,
}

impl Instance {
    pub fn new(_settings: &ApplicationSettings) -> Self {
        Self { dummy: 0 }
    }
}
