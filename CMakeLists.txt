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
include(version_info)

# setup sanitize build types
get_property(is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(is_multi_config)
    if(CMAKE_CONFIGURATION_TYPES)
        set(build_types ${CMAKE_CONFIGURATION_TYPES})
        list(TRANSFORM build_types TOUPPER)
        if("ASAN" IN_LIST build_types)
            setup_asan_flags()
        elseif("UBSAN" IN_LIST build_types)
            setup_ubsan_flags()
        elseif("TSAN" IN_LIST build_types)
            setup_tsan_flags()
        elseif("SANITIZE" IN_LIST build_types)
            setup_sanitize_flags()
        endif()
    endif()
else()
    if(CMAKE_BUILD_TYPE)
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
    endif()
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

# Statically link runtime library with MSVC
# https://cmake.org/cmake/help/latest/variable/CMAKE_MSVC_RUNTIME_LIBRARY.html
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<OR:$<CONFIG:Debug>,$<CONFIG:Asan>,$<CONFIG:UBSan>,$<CONFIG:Tsan>,$<CONFIG:Sanitize>>:Debug>")

# Statically link libgcc and libstdc++ with MINGW gcc
# https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html
if(MINGW AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    custom_check_cxx_compiler_flag("-static-libgcc" res1)
    custom_check_cxx_compiler_flag("-static-libstdc++" res2)
    if(res1 AND res2)
        set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
    else()
        message(WARNING "-static-libgcc/-static-libstdc++ reported as unsupported by compiler")
    endif()
endif()

# Export compile commands for external tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# IDE folders config
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "_CMake")

# Dependencies
include(fmt)
include(glfw)
include(glad)
include(opengl)
include(imgui)
include(implot)
include(impop)
include(imspinner)
include(ImGuiColorTextEdit)
include(ImGuiFileDialog)
include(ImGuiNotify)
include(nlohmann_json)
include(spdlog)
include(utfcpp)
include(scope_guard)
include(sigslots)
include(fonts)
include(expected)
include(PlatformFolders)
include(PEGTL)
include(tomlplusplus)
include(curl)

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
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources} ${includes})

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
  glad::glad_gl_core_33
  OpenGL::GL
  imgui::imgui
  implot::implot
  impop::impop
  imspinner::imspinner
  ImGuiColorTextEdit::ImGuiColorTextEdit
  ImGuiFileDialog::ImGuiFileDialog
  ImGuiNotify::ImGuiNotify
  nlohmann_json::nlohmann_json
  spdlog::spdlog
  utf8cpp::utf8cpp
  scope_guard::scope_guard
  Pal::Sigslot
  fonts::fonts
  tl::expected
  sago::platform_folders
  taocpp::pegtl
  tomlplusplus::tomlplusplus
  CURL::libcurl
  git_info
  version_info
)

# Set log level
target_compile_definitions(
  testgui PRIVATE
  SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE
  SPDLOG_NO_SOURCE_LOC
  TOML_EXCEPTIONS=0
)

# Build in C++20
target_compile_features(testgui PRIVATE cxx_std_20)

# Add warning flags
target_add_cxx_warning_flags(testgui)

# make GUI executable
set_target_properties(testgui PROPERTIES WIN32_EXECUTABLE ON)

# Visual studio config
set_property(
  DIRECTORY
  ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY
  VS_STARTUP_PROJECT testgui
)

# Generate format target
find_program(CLANG_FORMAT clang-format)
if(${CLANG_FORMAT} STREQUAL CLANG_FORMAT-NOTFOUND)
    message(WARNING "clang-format not found, format targets not generated")
else()
    message(STATUS "clang-format found: ${CLANG_FORMAT}")
    add_custom_target(
      format-testgui
      COMMAND "${CLANG_FORMAT}" -style=file -i ${sources} ${includes}
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
      VERBATIM
    )
    set_target_properties(format-testgui PROPERTIES FOLDER "format")
endif()

# Install
include(GNUInstallDirs)
install(TARGETS testgui)
