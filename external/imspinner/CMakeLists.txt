#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#

# Fix for CMake pre 3.20 behaviour, see CMP0118
set_source_files_properties(${global_generated_files_list} PROPERTIES GENERATED ON)

# Download imspinner
FetchContent_Populate(
  imspinner
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/imspinner-src"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/imspinner-build"
  SUBBUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/imspinner-subbuild"
  GIT_REPOSITORY "https://github.com/dalerank/imspinner"
  GIT_TAG "ff2eb8c3230debc878db689c9113498a7de97804"
  GIT_SHALLOW ON
  GIT_PROGRESS ON
)

# Declare imspinner
add_library(imspinner INTERFACE)
add_library(imspinner::imspinner ALIAS imspinner)

# Add includes
target_include_directories(
  imspinner SYSTEM INTERFACE
  "${imspinner_SOURCE_DIR}/"
)

# Add definitions
target_compile_definitions(
  imgui PUBLIC
  IMSPINNER_DEMO
)

# Add dependencies
target_link_libraries(
  imspinner INTERFACE
  imgui::imgui
)
