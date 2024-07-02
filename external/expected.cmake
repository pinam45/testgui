#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#

# Fix for CMake pre 3.20 behaviour, see CMP0118
set_source_files_properties(${global_generated_files_list} PROPERTIES GENERATED ON)

if(TARGET tl::expected)
    return()
endif()

message(CHECK_START "external: configuring expected")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

# Download
FetchContent_Populate(
  expected
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/expected-src"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/expected-build"
  SUBBUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/expected-subbuild"
  GIT_REPOSITORY "https://github.com/TartanLlama/expected"
  GIT_TAG "v1.1.0"
  GIT_SHALLOW ON
  GIT_PROGRESS ON
)

# Import
set(BUILD_TESTING OFF)
set(EXPECTED_BUILD_TESTS OFF)
set(EXPECTED_BUILD_PACKAGE_DEB OFF)
add_subdirectory(
  ${expected_SOURCE_DIR}
  EXCLUDE_FROM_ALL
  SYSTEM
)
if(NOT TARGET tl::expected)
    message(FATAL_ERROR "tl::expected target is missing")
endif()

list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "done")
