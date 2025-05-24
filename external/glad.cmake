#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#

# Fix for CMake pre 3.20 behaviour, see CMP0118
set_source_files_properties(${global_generated_files_list} PROPERTIES GENERATED ON)

if(TARGET glad::glad_gl_core_33)
    return()
endif()

message(CHECK_START "external: configuring glad")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

# Download
FetchContent_Populate(
  glad
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/glad-src"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/glad-build"
  SUBBUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/glad-subbuild"
  GIT_REPOSITORY "https://github.com/Dav1dde/glad"
  GIT_TAG "v2.0.8"
  GIT_SHALLOW ON
  GIT_PROGRESS ON
  UPDATE_DISCONNECTED ON
)

# requirements:
# pip install jinja2

# Import
add_subdirectory(
  ${glad_SOURCE_DIR}/cmake
  EXCLUDE_FROM_ALL
  SYSTEM
)
glad_add_library(glad_gl_core_33 REPRODUCIBLE API gl:core=3.3)
#glad_add_library(glad_gl_core_33 REPRODUCIBLE MX API gl:core=3.3)
set_target_properties(glad_gl_core_33 PROPERTIES FOLDER external)
add_library(glad::glad_gl_core_33 ALIAS glad_gl_core_33)

list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "done")
