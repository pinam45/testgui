#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#

# Fix for CMake pre 3.20 behaviour, see CMP0118
set_source_files_properties(${global_generated_files_list} PROPERTIES GENERATED ON)

if(TARGET glfw::glfw)
    return()
endif()

message(CHECK_START "external: configuring glfw")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

# Download
FetchContent_Populate(
  glfw
  GIT_REPOSITORY "https://github.com/glfw/glfw"
  GIT_TAG "3.3.8"
  GIT_SHALLOW ON
  GIT_PROGRESS ON
)

# requirements:
# apt install libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev

# Import
set(BUILD_TESTING OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
set(GLFW_VULKAN_STATIC OFF)
add_subdirectory(
  ${glfw_SOURCE_DIR}
  EXCLUDE_FROM_ALL
  SYSTEM
)
if(NOT TARGET glfw)
    message(FATAL_ERROR "glfw target is missing")
endif()
set_target_properties(glfw PROPERTIES FOLDER external)
add_library(glfw::glfw ALIAS glfw)

list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "done")
