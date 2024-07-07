#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#

# Fix for CMake pre 3.20 behaviour, see CMP0118
set_source_files_properties(${global_generated_files_list} PROPERTIES GENERATED ON)

# Download implot
FetchContent_Populate(
  implot
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/implot-src"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/implot-build"
  SUBBUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/implot-subbuild"
  GIT_REPOSITORY "https://github.com/epezent/implot"
  GIT_TAG "f156599faefe316f7dd20fe6c783bf87c8bb6fd9"
  #GIT_SHALLOW ON
  GIT_PROGRESS ON
  UPDATE_DISCONNECTED ON
)

# Declare implot
add_library(implot STATIC)
add_library(implot::implot ALIAS implot)

# Add sources
file(
  GLOB sources
  "${implot_SOURCE_DIR}/*.cpp"
  "${implot_SOURCE_DIR}/*.h"
)
target_sources(implot PRIVATE ${sources})

# Add includes
target_include_directories(
  implot SYSTEM PUBLIC
  "${implot_SOURCE_DIR}"
)

# Add dependencies
target_link_libraries(
  implot PUBLIC
  imgui::imgui
)

# Configure
target_compile_features(implot PRIVATE cxx_std_17)
set_target_properties(implot PROPERTIES FOLDER external)
