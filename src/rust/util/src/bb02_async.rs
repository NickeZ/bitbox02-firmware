// SPDX-License-Identifier: Apache-2.0

use alloc::boxed::Box;
use core::pin::Pin;
use core::task::{Context, Poll};

/// Task is the top-level future which can be polled by an executor.
/// Note that other futures awaited inside do not have to be pinned.
/// The 'a lifetime allows to spin a boxed/pinned future that is not
/// 'static, or a future with non-'static input param references.
pub type Task<'a, O> = Pin<Box<dyn core::future::Future<Output = O> + 'a>>;

/// A primitive poll invocation for a task, with no waking functionality.
pub fn spin<O>(task: &mut Task<O>) -> Poll<O> {
    // TODO: statically allocate the context.
    let waker = crate::waker_fn::waker_fn(|| {});
    let context = &mut Context::from_waker(&waker);
    task.as_mut().poll(context)
}

#[cfg(feature = "testing")]
pub use futures_lite::future::block_on;
