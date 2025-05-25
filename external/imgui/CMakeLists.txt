#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#

# Fix for CMake pre 3.20 behaviour, see CMP0118
set_source_files_properties(${global_generated_files_list} PROPERTIES GENERATED ON)

# Download imgui
FetchContent_Populate(
  imgui
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/imgui-src"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/imgui-build"
  SUBBUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/imgui-subbuild"
  GIT_REPOSITORY "https://github.com/ocornut/imgui"
  GIT_TAG "v1.90.9-docking"
  GIT_SHALLOW ON
  GIT_PROGRESS ON
  UPDATE_DISCONNECTED ON
)

# Declare imgui
add_library(imgui STATIC)
add_library(imgui::imgui ALIAS imgui)

# Add sources
file(
  GLOB sources
  "${imgui_SOURCE_DIR}/*.cpp"
  "${imgui_SOURCE_DIR}/*.h"
  "${imgui_SOURCE_DIR}/misc/cpp/*.cpp"
  "${imgui_SOURCE_DIR}/misc/cpp/*.h"
  "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h"
  "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
  "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h"
  "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
)
target_sources(imgui PRIVATE ${sources})

# Add includes
target_include_directories(
  imgui SYSTEM PUBLIC
  "${imgui_SOURCE_DIR}"
  "${imgui_SOURCE_DIR}/misc/cpp/"
)

# Add dependencies
target_link_libraries(
  imgui PUBLIC
  glfw::glfw
  glad::glad_gl_core_33
  OpenGL::GL
)

# Add definitions
target_compile_definitions(
  imgui PUBLIC
  IMGUI_IMPL_OPENGL_LOADER_GL3W
  IMGUI_DISABLE_OBSOLETE_FUNCTIONS
  IMGUI_DEFINE_MATH_OPERATORS
)

# Configure
target_compile_features(imgui PRIVATE cxx_std_98)
set_target_properties(imgui PROPERTIES FOLDER external)
