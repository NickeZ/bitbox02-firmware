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

pub mod cancel;
pub mod confirm;
pub mod menu;
#[cfg_attr(feature = "c-unit-testing", path = "workflow/mnemonic_c_unit_tests.rs")]
pub mod mnemonic;
pub mod pairing;
pub mod password;
pub mod sdcard;
pub mod status;
#[cfg(feature = "testing")]
pub mod testing;
pub mod transaction;
pub mod trinary_choice;
pub mod trinary_input_string;
pub mod unlock;
pub mod verify_message;

#[allow(async_fn_in_trait)]
pub trait Workflows {
    async fn confirm(&mut self, params: &confirm::Params<'_>) -> Result<(), confirm::UserAbort>;

    async fn verify_recipient(
        &mut self,
        recipient: &str,
        amount: &str,
    ) -> Result<(), transaction::UserAbort>;

    async fn verify_total_fee(
        &mut self,
        total: &str,
        fee: &str,
        longtouch: bool,
    ) -> Result<(), transaction::UserAbort>;

    async fn status(&mut self, title: &str, status_success: bool);
}

pub struct RealWorkflows;

impl Workflows for RealWorkflows {
    #[inline(always)]
    async fn confirm(&mut self, params: &confirm::Params<'_>) -> Result<(), confirm::UserAbort> {
        confirm::confirm(params).await
    }

    #[inline(always)]
    async fn verify_recipient(
        &mut self,
        recipient: &str,
        amount: &str,
    ) -> Result<(), transaction::UserAbort> {
        transaction::verify_recipient(recipient, amount).await
    }

    #[inline(always)]
    async fn verify_total_fee(
        &mut self,
        total: &str,
        fee: &str,
        longtouch: bool,
    ) -> Result<(), transaction::UserAbort> {
        transaction::verify_total_fee(total, fee, longtouch).await
    }

    #[inline(always)]
    async fn status(&mut self, title: &str, status_success: bool) {
        status::status(title, status_success).await
    }
}
