#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#

# Fix for CMake pre 3.20 behaviour, see CMP0118
if(NOT global_generated_files_list_inited)
    set(global_generated_files_list_inited ON)
    set(global_generated_files_list "" CACHE INTERNAL "" FORCE)
endif()

cmake_minimum_required(VERSION 3.25)
project(
  testgui
  VERSION 0.0.1
  DESCRIPTION "test GUI"
  LANGUAGES CXX C
)
include(FetchContent)

# Set CMake modules path
list(APPEND
  CMAKE_MODULE_PATH
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules"
  "${CMAKE_CURRENT_SOURCE_DIR}/external"
)

# Flags module
include(flags)
include(git_info)

# setup sanitize build types
string(TOUPPER ${CMAKE_BUILD_TYPE} build_type)
if(build_type STREQUAL "ASAN")
    setup_asan_flags()
elseif(build_type STREQUAL "UBSAN")
    setup_ubsan_flags()
elseif(build_type STREQUAL "TSAN")
    setup_tsan_flags()
elseif(build_type STREQUAL "SANITIZE")
    setup_sanitize_flags()
endif()

# Disable in source build
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# Use strict C standard
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Use strict C++ standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for external tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
include(fmt)
include(glfw)
include(gl3w)
include(opengl)
include(imgui)
include(implot)
include(impop)
include(imspinner)
include(ImGuiColorTextEdit)
include(ImGuiNotify)
include(nlohmann_json)
include(spdlog)
include(utfcpp)
include(scope_guard)
include(sigslots)
include(fonts)
include(expected)
include(PlatformFolders)

# Declare target
add_executable(testgui)

# Get sources
file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE includes "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")

# Add sources
target_sources(
  testgui PRIVATE
  ${sources} ${includes}
)

# Add includes
target_include_directories(
  testgui PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# Link dependencies
target_link_libraries(
  testgui PRIVATE
  fmt::fmt
  glfw::glfw
  gl3w::gl3w
  OpenGL::GL
  imgui::imgui
  implot::implot
  impop::impop
  imspinner::imspinner
  ImGuiColorTextEdit::ImGuiColorTextEdit
  ImGuiNotify::ImGuiNotify
  nlohmann_json::nlohmann_json
  spdlog::spdlog
  utf8::cpp
  scope_guard::scope_guard
  Pal::Sigslot
  fonts::fonts
  tl::expected
  sago::platform_folders
  git_info
)

# Set log level
target_compile_definitions(
  testgui PRIVATE
  SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE
)

# Build in C++17
target_compile_features(testgui PRIVATE cxx_std_17)

# Add warning flags
target_add_cxx_warning_flags(testgui)
