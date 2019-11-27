// Copyright 2019 Shift Cryptosecurity AG
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

use core::fmt::Write;
use core::time::Duration;

use arrayvec::ArrayString;

use crate::c_char;
use crate::error::Error;
use crate::util::{FixedCString, Ipv4Addr};

/// This determines if the LED should be turned off in some states.
#[derive(Debug, Clone)]
#[repr(u8)]
#[allow(dead_code)] // We are setting this with transmute
pub enum StatusLedMode {
    /// Always show the LED
    Always,
    /// Only show the led in the Working, Warning and Error states
    OnWorking,
    /// Only show the led in the Warning and Error states
    OnWarning,
    /// Only show the led in the Error state
    OnError,
}

#[derive(Debug, Clone)]
#[repr(u8)]
#[allow(dead_code)] // We are setting this with transmute
pub enum StatusScreenMode {
    /// Always show the screen
    Always,
    /// Only show the screen in the Working, Warning and Error states
    OnWorking,
    /// Only show the screen in the Warning and Error states
    OnWarning,
    /// Only show the screen in the Error state
    OnError,
}

// Fields that require validation must be set via setter functions
pub struct Config {
    pub(crate) status_led_mode: StatusLedMode,
    pub(crate) status_screen_mode: StatusScreenMode,
    pub(crate) default_display_duration: Duration,
    hostname: Option<ArrayString<[u8; 64]>>,
    pub(crate) ip: Option<Ipv4Addr>,
    port: Option<u16>,
    onion: Option<ArrayString<[u8; 64]>>, // TODO(nc): change to 62 when we have const generics
}

impl Config {
    pub const fn new() -> Config {
        Config {
            status_led_mode: StatusLedMode::Always,
            status_screen_mode: StatusScreenMode::OnWarning,
            default_display_duration: Duration::from_secs(10),
            hostname: None,
            ip: None,
            port: None,
            onion: None,
        }
    }

    pub fn set_hostname(&mut self, hostname: &str) -> Result<(), Error> {
        if !hostname.is_ascii() {
            return Err(Error::InvalidHostname);
        }
        if hostname.len() < 1 || hostname.len() > 63 {
            return Err(Error::InvalidHostname);
        }
        // Since we checked that the string contains at least one character we can safely unwrap
        if hostname.chars().nth(0).unwrap() == '-' || hostname.chars().last().unwrap() == '-' {
            return Err(Error::InvalidHostname);
        }
        if hostname.chars().any(|c| !c.is_alphanumeric() && c != '-') {
            return Err(Error::InvalidHostname);
        }
        self.hostname = Some(ArrayString::from(hostname).expect("Buffer to short"));
        Ok(())
    }

    pub fn get_hostname(&self) -> Option<&str> {
        self.hostname.as_ref().map(|h| h.as_str())
    }

    pub fn set_port(&mut self, port: u16) {
        self.port = match port {
            0 => None,
            port => Some(port),
        };
    }

    pub fn get_port(&self) -> Option<u16> {
        self.port
    }

    pub fn set_onion(&mut self, onion: Option<&str>) -> Result<(), Error> {
        const EXT_LEN: usize = 6; // sizeof ".onion"
        match onion {
            None => self.onion = None,
            Some(onion) => {
                if !onion.ends_with(".onion") {
                    return Err(Error::InvalidOnionAddress);
                }
                if onion.len() != 16 + EXT_LEN || onion.len() != 56 + EXT_LEN {
                    return Err(Error::InvalidOnionAddress);
                }
                if !onion.is_ascii() {
                    return Err(Error::InvalidOnionAddress);
                }
                {
                    // Onion address without ".onion"
                    let address = &onion[..(onion.len() - EXT_LEN)];
                    if address.chars().any(|c| !c.is_alphanumeric()) {
                        return Err(Error::InvalidOnionAddress);
                    }
                }
                self.onion = Some(ArrayString::from(onion).expect("Buffer to short"));
            }
        }
        Ok(())
    }

    pub unsafe fn get_singleton() -> &'static Config {
        &CONFIG
    }

    pub unsafe fn get_singleton_mut() -> &'static mut Config {
        &mut CONFIG
    }
}

//
// C-API
//

// aaaah, global!
static mut CONFIG: Config = Config::new();

#[no_mangle]
pub extern "C" fn bitboxbase_config_led_mode_get() -> StatusLedMode {
    let config = unsafe { Config::get_singleton() };
    config.status_led_mode.clone()
}

#[no_mangle]
pub extern "C" fn bitboxbase_config_ip_get(res: *mut c_char, res_len: usize) {
    // It is not safe to call any functions that also touch CONFIG at the same time
    let config = unsafe { Config::get_singleton() };
    let buf = unsafe { core::slice::from_raw_parts_mut(res, res_len) };
    let mut fcstring = FixedCString::new(buf);

    if let Some(ip) = &config.ip {
        let _ = write!(fcstring, "{}", ip);
    } else {
        let _ = write!(fcstring, "unknown");
    }
}
