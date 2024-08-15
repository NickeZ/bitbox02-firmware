# Copyright 2019 Shift Cryptosecurity AG
# Copyright 2020 Shift Crypto AG
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

FROM shiftcrypto/ubuntu:1

# Install gcc8-arm-none-eabi
RUN wget -O gcc.tar.bz2 https://developer.arm.com/-/media/Files/downloads/gnu-rm/8-2018q4/gcc-arm-none-eabi-8-2018-q4-major-linux.tar.bz2?revision=d830f9dd-cd4f-406d-8672-cca9210dd220?product=GNU%20Arm%20Embedded%20Toolchain,64-bit,,Linux,8-2018-q4-major &&\
    echo "fb31fbdfe08406ece43eef5df623c0b2deb8b53e405e2c878300f7a1f303ee52 gcc.tar.bz2" | sha256sum -c &&\
    tar -xjf gcc.tar.bz2 -C /usr/local --strip-components=1 &&\
    rm gcc.tar.bz2 /root/.wget-hsts

# Set gcc-10 as the default gcc
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 100 && rm /var/log/alternatives.log
RUN update-alternatives --install /usr/bin/gcov gcov /usr/bin/gcov-10 100 && rm /var/log/alternatives.log

# Make Python3 the default
RUN update-alternatives --install /usr/bin/python python /usr/bin/python3 1 && rm /var/log/alternatives.log

# Python modules
RUN --mount=source=py/bitbox02,target=/mnt/bitbox02,rw \
    pip install --no-compile --no-cache-dir /mnt/bitbox02
RUN --mount=source=py/requirements.txt,target=/mnt/requirements.txt \
    pip install --no-compile --no-cache-dir --upgrade --requirement /mnt/requirements.txt

# Python modules for CI
RUN pip install --no-compile --no-cache-dir --upgrade \
    pylint==2.13.9 \
    pylint-protobuf==0.20.2 \
    black==22.3.0 \
    mypy==0.960 \
    mypy-protobuf==3.2.0 \
    setuptools==41.2.0 \
    wheel==0.33.6 \
    twine==1.15.0 \
    gcovr==7.2

#Install protoc from release, because the version available on the repo is too old
RUN mkdir -p /opt/protoc && \
    curl -L0 https://github.com/protocolbuffers/protobuf/releases/download/v21.2/protoc-21.2-linux-x86_64.zip -o /tmp/protoc-21.2-linux-x86_64.zip && \
    unzip /tmp/protoc-21.2-linux-x86_64.zip -d /opt/protoc &&\
    rm /tmp/protoc-21.2-linux-x86_64.zip
ENV PATH=/opt/protoc/bin:$PATH

# Install Go, used for the tools in tools/go and for test/gounittest
ENV GOPATH=/opt/go
ENV GOROOT=/opt/go_dist/go
ENV PATH=$GOROOT/bin:$GOPATH/bin:$PATH
RUN mkdir -p /opt/go_dist && \
    curl https://dl.google.com/go/go1.19.3.linux-amd64.tar.gz | tar -xz -C /opt/go_dist

# Install lcov from release (the one from the repos is too old).
RUN curl -L https://github.com/linux-test-project/lcov/releases/download/v1.14/lcov-1.14.tar.gz | tar -xz -C /opt
ENV PATH=/opt/lcov-1.14/bin:$PATH

# Install rust compiler
# Since bindgen embeds information about its target directory, use a deterministic path for it.
ENV PATH=/opt/cargo/bin:$PATH
ENV RUSTUP_HOME=/opt/rustup
RUN --mount=source=tools/prost-build-proto,target=/mnt/prost-build-proto,rw \
    --mount=source=src/rust/rust-toolchain.toml,target=/mnt/rust-toolchain.toml \
    curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs |\
    CARGO_HOME=/opt/cargo sh -s -- --default-toolchain $(grep -oP '(?<=channel = ")[^"]+' /mnt/rust-toolchain.toml) -y &&\
    rustup target add thumbv7em-none-eabi &&\
    rustup component add rustfmt &&\
    rustup component add clippy &&\
    rustup component add rust-src &&\
    CARGO_HOME=/opt/cargo cargo install cbindgen --version 0.26.0 --locked &&\
    CARGO_HOME=/opt/cargo cargo install bindgen-cli --version 0.69.4 --locked --target-dir=/tmp/bindgen-target &&\
    CARGO_HOME=/opt/cargo cargo install --path /mnt/prost-build-proto --locked &&\
    rm -r /tmp/bindgen-target /opt/cargo/registry/index /opt/cargo/.global-cache

# Until cargo vendor supports vendoring dependencies of the rust std libs we
# need a copy of this file next to the toml file. It also has to be world
# writable so that invocations of `cargo vendor` can update it. Below is the
# tracking issue for `cargo vendor` to support rust std libs.
# https://github.com/rust-lang/wg-cargo-std-aware/issues/23
RUN cp "$(rustc --print=sysroot)/lib/rustlib/src/rust/Cargo.lock" "$(rustc --print=sysroot)/lib/rustlib/src/rust/library/test/" &&\
    chmod 777 $(rustc --print=sysroot)/lib/rustlib/src/rust/library/test/Cargo.lock
