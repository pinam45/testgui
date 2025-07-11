#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#

# Fix for CMake pre 3.20 behaviour, see CMP0118
set_source_files_properties(${global_generated_files_list} PROPERTIES GENERATED ON)

if(TARGET sago::platform_folders)
    return()
endif()

message(CHECK_START "external: configuring PlatformFolders")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

# Patch file path
file(TO_CMAKE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/PlatformFolders/fix.patch" PATCH_FILE)

# Download
FetchContent_Populate(
  platformfolders
  GIT_REPOSITORY "https://github.com/sago007/platformfolders"
  GIT_TAG "4.2.0"
  GIT_SHALLOW ON
  GIT_PROGRESS ON
  PATCH_COMMAND ${CMAKE_COMMAND}
  -D PATCH_FILE=${PATCH_FILE}
  -D SOURCE_DIR=<SOURCE_DIR>
  -P ${CMAKE_SOURCE_DIR}/cmake/apply_patch.cmake
)

# Import
set(BUILD_TESTING OFF)
set(PLATFORMFOLDERS_BUILD_SHARED_LIBS OFF)
set(PLATFORMFOLDERS_BUILD_TESTING OFF)
set(PLATFORMFOLDERS_ENABLE_INSTALL OFF)
add_subdirectory(
  ${platformfolders_SOURCE_DIR}
  EXCLUDE_FROM_ALL
  SYSTEM
)
if(NOT TARGET platform_folders OR NOT TARGET sago::platform_folders)
    message(FATAL_ERROR "sago::platform_folders target is missing")
endif()
set_target_properties(platform_folders PROPERTIES FOLDER external)

list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "done")
