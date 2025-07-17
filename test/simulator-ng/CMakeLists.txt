# Copyright 2024 Shift Cryptosecurity AG
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

#-----------------------------------------------------------------------------
# Build bitbox lib to use in tests

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-parameter -Wno-missing-prototypes -Wno-missing-declarations -Wno-implicit-function-declaration -Wno-bad-function-cast")

set(DBB-FILTERED-SOURCES
  ${DBB-FIRMWARE-SOURCES}
  ${DBB-FIRMWARE-UI-SOURCES}
  ${FIRMWARE-U2F-SOURCES}
  ${DBB-FIRMWARE-USB-SOURCES}
  ${PLATFORM-BITBOX02-PLUS-SOURCES}
  "${CMAKE_SOURCE_DIR}/external/asf4-drivers/hal/utils/src/utils_ringbuffer.c"
  )

set(IGNORE_SOURCES
    "src/firmware_main_loop.c"
    "src/memory/nvmctrl.c"
    "src/memory/smarteeprom.c"
    "src/memory/mpu.c"
    "src/memory/spi_mem.c"
    "src/uart.c"
    )

# Exclude some files which depends on the hardware.
foreach(SOURCEFILE ${IGNORE_SOURCES})
    list(FILTER DBB-FILTERED-SOURCES EXCLUDE REGEX ".*/${SOURCEFILE}$")
endforeach()

# We create a CMake "object library" to track all the compiled sources so that
# they can be reused between a normal library and the manually crafted "merged"
# library.
# See https://cmake.org/cmake/help/v3.10/command/add_library.html#object-libraries
add_library(bitbox_objects-simulator-ng
  OBJECT
  ${DBB-FILTERED-SOURCES}
  ${ETHEREUM-SOURCES}
)

add_library(bitbox-simulator-ng
  STATIC
  $<TARGET_OBJECTS:bitbox_objects-simulator-ng>
  $<TARGET_OBJECTS:hardware-fakes-objects>
)

# Here we create the "merged" library, which starts with the c-lib created by
# cargo in the "bitbox02-rust-c" project. That project produces a c-lib without
# mangled symbols so that it is easier to use from C. We then extend that
# library with all the object files declared in "bitbox_objects-simulator", all the code
# we want to be able to call from tests.
# By linking to "bitbox_merged-simulator" in the rust unit tests we get access to all our
# code. (That linkage is done by cargo using a "build.rs" script in the
# bitbox02 rust project.)
add_custom_command(
  OUTPUT ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libbitbox_merged-simulator-ng.a
  DEPENDS c-simulator_rust_c bitbox_objects-simulator $<TARGET_OBJECTS:bitbox_objects-simulator> $<TARGET_OBJECTS:hardware-fakes-objects>
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PROPERTY:simulator-ng_rust_c,IMPORTED_LOCATION> ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libbitbox_merged-simulator-ng.a
  COMMAND ar q ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libbitbox_merged-simulator-ng.a $<TARGET_OBJECTS:bitbox_objects-simulator>
  VERBATIM
  COMMAND_EXPAND_LISTS
)

add_custom_target(bitbox_merged-simulator-ng DEPENDS ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libbitbox_merged-simulator-ng.a)

target_include_directories(
  bitbox_objects-simulator-ng
  SYSTEM PUBLIC
  ${CMAKE_SOURCE_DIR}/external
  ${CMAKE_SOURCE_DIR}/external/asf4-drivers/hal/utils/include
  ${CMAKE_SOURCE_DIR}/external/fatfs/source
  $<TARGET_PROPERTY:wallycore,INTERFACE_INCLUDE_DIRECTORIES>
  $<TARGET_PROPERTY:secp256k1,INTERFACE_INCLUDE_DIRECTORIES>
  $<TARGET_PROPERTY:fatfs,INTERFACE_INCLUDE_DIRECTORIES>
)
target_include_directories(
  bitbox_objects-simulator-ng
  PUBLIC
  ${INCLUDES}
  ${CMAKE_CURRENT_SOURCE_DIR}/../unit-test/framework/includes
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}/src
)

target_include_directories(
  bitbox-simulator-ng
  SYSTEM PUBLIC
  ${CMAKE_SOURCE_DIR}/external
  ${CMAKE_SOURCE_DIR}/external/asf4-drivers/hal/utils/include
  ${CMAKE_SOURCE_DIR}/external/fatfs/source
  $<TARGET_PROPERTY:wallycore,INTERFACE_INCLUDE_DIRECTORIES>
  $<TARGET_PROPERTY:secp256k1,INTERFACE_INCLUDE_DIRECTORIES>
  $<TARGET_PROPERTY:fatfs,INTERFACE_INCLUDE_DIRECTORIES>
)

target_include_directories(
  bitbox-simulator-ng
  PUBLIC
  ${INCLUDES}
  ${CMAKE_CURRENT_SOURCE_DIR}/../unit-test/framework/includes
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}/src
)

#add_dependencies(bitbox_objects-simulator
#  rust-cbindgen
#  wallycore
#  secp256k1
#  fatfs
#  sd-mock-simulator
#)
#target_link_libraries(bitbox-simulator PRIVATE ${LIBBITBOX02_RUST} "-lm")

# _UNIT_TEST_ is used by ASF4 to not cross compile

target_compile_definitions(bitbox_objects-simulator-ng PUBLIC "PRODUCT_BITBOX_MULTI=1" "APP_BTC=1" "APP_LTC=1" "APP_U2F=1" "APP_ETH=1")
target_compile_definitions(bitbox_objects-simulator-ng PUBLIC TESTING _UNIT_TEST_)

# Since wallycore is an external projects we need to specify the dependency
# add_dependencies(bitbox_objects-simulator libwally-core)
# 
# target_compile_definitions(bitbox-simulator PUBLIC "PRODUCT_BITBOX_MULTI=1" "APP_BTC=1" "APP_LTC=1" "APP_U2F=1" "APP_ETH=1")
# target_compile_definitions(bitbox-simulator PUBLIC TESTING _UNIT_TEST_)
# 
# target_link_libraries(bitbox-simulator
#   PUBLIC
#     secp256k1
#   PRIVATE
#     wallycore
#     fatfs
#     sd-mock-simulator
# )

#if(SANITIZE_ADDRESS)
#  target_compile_options(bitbox_objects-simulator PUBLIC "-fsanitize=address")
#  target_compile_options(bitbox-simulator PUBLIC "-fsanitize=address")
#endif()
#if(SANTIZE_UNDEFINED)
#  target_compile_options(bitbox_objects-simulator PUBLIC "-fsanitize=undefined")
#  target_compile_options(bitbox-simulator PUBLIC "-fsanitize=undefined")
#endif()

#-----------------------------------------------------------------------------
# Simulator

add_custom_command(
    OUTPUT target/release/simulator
    COMMAND cargo build --release
)
add_custom_target(simulator-ng
    DEPENDS target/release/simulator
)
#add_executable(simulator simulator.c framework/eh_personality.c)
## asan must be first library in linking order
#target_link_libraries(simulator PRIVATE
#  $<$<BOOL:${SANITIZE_ADDRESS}>:asan>
#  $<$<BOOL:${SANITIZE_UNDEFINED}>:-fsanitize=undefined>
#  $<$<NOT:$<PLATFORM_ID:Darwin>>:-Wl,--start-group>
#  c-unit-tests_rust_c
#  bitbox-simulator
#  $<$<NOT:$<PLATFORM_ID:Darwin>>:-Wl,--end-group>
#  ""
#  ${SDL2_LDFLAGS}
#)
#
#target_include_directories(
#  simulator
#  SYSTEM PRIVATE
#  ${SDL2_INCLUDE_DIRS}
#)

#add_custom_command(
#  TARGET simulator POST_BUILD
#  COMMAND ${CMAKE_STRIP} simulator
#  WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
#  COMMENT "\nStripping simulator binary"
#)

#if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
#    target_link_libraries(simulator PRIVATE "-sectcreate __DATA_CONST __bg_png ${CMAKE_CURRENT_SOURCE_DIR}/bg.png")
#else()
#add_custom_command(
#  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/bg.o
#  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/bg.png ${CMAKE_CURRENT_BINARY_DIR}
#  COMMAND ${CMAKE_OBJCOPY} -Ibinary -Odefault --rename-section .data=.rodata,alloc,load,readonly,data,contents bg.png bg.o
#  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/bg.png
#  COMMENT "Convert bg to object file using ${CMAKE_OBJCOPY}")

#add_custom_target(
#  simulator-bg
#  DEPENDS bg.o
#)
#add_dependencies(simulator simulator-bg)
#
#target_link_libraries(simulator PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/bg.o)
#endif()
