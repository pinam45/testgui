#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#

# Fix for CMake pre 3.20 behaviour, see CMP0118
set_source_files_properties(${global_generated_files_list} PROPERTIES GENERATED ON)

if(TARGET fmt::fmt)
    return()
endif()

message(CHECK_START "external: configuring fmt")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

# Download
FetchContent_Populate(
  fmt
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/fmt-src"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/fmt-build"
  SUBBUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/fmt-subbuild"
  GIT_REPOSITORY "https://github.com/fmtlib/fmt"
  GIT_TAG "11.2.0"
  GIT_SHALLOW ON
  GIT_PROGRESS ON
  UPDATE_DISCONNECTED ON
)

# Import
set(BUILD_TESTING OFF)
add_subdirectory(
  ${fmt_SOURCE_DIR}
  EXCLUDE_FROM_ALL
  SYSTEM
)
if(NOT TARGET fmt OR NOT TARGET fmt::fmt)
    message(FATAL_ERROR "fmt::fmt target is missing")
endif()
set_target_properties(fmt PROPERTIES FOLDER external)

list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "done")
