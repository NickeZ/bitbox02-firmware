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

set(HARDWARE-FAKES-SOURCES
  src/mock_cipher.c
  src/mock_diskio.c
  src/mock_memory.c
  src/mock_screen.c
  src/mock_securechip.c
  src/mock_smarteeprom.c
  src/mock_spi_mem.c
)

# We create a CMake "object library" to track all the compiled sources so that
# they can be reused between a normal library and the manually crafted "merged"
# library.
# See https://cmake.org/cmake/help/v3.10/command/add_library.html#object-libraries
add_library(hardware-fakes-objects
  OBJECT
  ${HARDWARE-FAKES-SOURCES}
)

target_include_directories(
  hardware-fakes-objects
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  $<TARGET_PROPERTY:fatfs,INTERFACE_INCLUDE_DIRECTORIES>
)
target_include_directories(
  hardware-fakes-objects
  PRIVATE
  ${CMAKE_SOURCE_DIR}/src
  $<TARGET_PROPERTY:wallycore,INTERFACE_INCLUDE_DIRECTORIES>
)

target_compile_definitions(hardware-fakes-objects PUBLIC "PRODUCT_BITBOX_MULTI=1" "APP_BTC=1" "APP_LTC=1" "APP_U2F=1" "APP_ETH=1")
target_compile_definitions(hardware-fakes-objects PUBLIC TESTING _UNIT_TEST_)

target_link_libraries(hardware-fakes-objects PRIVATE wallycore)

add_library(hardware-fakes
  STATIC
  $<TARGET_OBJECTS:hardware-fakes-objects>
)

#target_include_directories(
#  hardware-fakes
#  SYSTEM PUBLIC
#  ${CMAKE_SOURCE_DIR}/external
#  ${CMAKE_SOURCE_DIR}/external/asf4-drivers/hal/utils/include
#  ${CMAKE_SOURCE_DIR}/external/fatfs/source
#  $<TARGET_PROPERTY:wallycore,INTERFACE_INCLUDE_DIRECTORIES>
#  $<TARGET_PROPERTY:secp256k1,INTERFACE_INCLUDE_DIRECTORIES>
#  $<TARGET_PROPERTY:fatfs,INTERFACE_INCLUDE_DIRECTORIES>
#)
#target_include_directories(
#  bitbox_objects-simulator
#  PUBLIC
#  ${INCLUDES}
#  ${CMAKE_CURRENT_SOURCE_DIR}/../unit-test/framework/includes
#  ${CMAKE_CURRENT_SOURCE_DIR}
#  ${CMAKE_BINARY_DIR}/src
#)
#
#target_include_directories(
#  bitbox-simulator
#  SYSTEM PUBLIC
#  ${CMAKE_SOURCE_DIR}/external
#  ${CMAKE_SOURCE_DIR}/external/asf4-drivers/hal/utils/include
#  ${CMAKE_SOURCE_DIR}/external/fatfs/source
#  $<TARGET_PROPERTY:wallycore,INTERFACE_INCLUDE_DIRECTORIES>
#  $<TARGET_PROPERTY:secp256k1,INTERFACE_INCLUDE_DIRECTORIES>
#  $<TARGET_PROPERTY:fatfs,INTERFACE_INCLUDE_DIRECTORIES>
#)
#
#target_include_directories(
#  bitbox-simulator
#  PUBLIC
#  ${INCLUDES}
#  ${CMAKE_CURRENT_SOURCE_DIR}/../unit-test/framework/includes
#  ${CMAKE_CURRENT_SOURCE_DIR}
#  ${CMAKE_BINARY_DIR}/src
#)
#
#add_dependencies(bitbox_objects-simulator
#  rust-cbindgen
#  wallycore
#  secp256k1
#  fatfs
#  sd-mock-simulator
#)
#target_link_libraries(bitbox-simulator PRIVATE ${LIBBITBOX02_RUST} "-lm")

# _UNIT_TEST_ is used by ASF4 to not cross compile

#target_compile_definitions(bitbox_objects-simulator PUBLIC "PRODUCT_BITBOX_MULTI=1" "APP_BTC=1" "APP_LTC=1" "APP_U2F=1" "APP_ETH=1")
#target_compile_definitions(bitbox_objects-simulator PUBLIC TESTING _UNIT_TEST_)
#
## Since wallycore is an external projects we need to specify the dependency
#add_dependencies(bitbox_objects-simulator libwally-core)
#
#target_compile_definitions(bitbox-simulator PUBLIC "PRODUCT_BITBOX_MULTI=1" "APP_BTC=1" "APP_LTC=1" "APP_U2F=1" "APP_ETH=1")
#target_compile_definitions(bitbox-simulator PUBLIC TESTING _UNIT_TEST_)
#
#target_link_libraries(bitbox-simulator
#  PUBLIC
#    secp256k1
#  PRIVATE
#    wallycore
#    fatfs
#    sd-mock-simulator
#)

if(SANITIZE_ADDRESS)
  target_compile_options(hardware-fakes-objects PUBLIC "-fsanitize=address")
endif()
if(SANTIZE_UNDEFINED)
  target_compile_options(hardware-fakes-objects PUBLIC "-fsanitize=undefined")
endif()
