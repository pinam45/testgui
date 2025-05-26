#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#

# Fix for CMake pre 3.20 behaviour, see CMP0118
set_source_files_properties(${global_generated_files_list} PROPERTIES GENERATED ON)

if(TARGET tomlplusplus::tomlplusplus)
    return()
endif()

message(CHECK_START "external: configuring tomlplusplus")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

# Download
FetchContent_Populate(
  tomlplusplus
  GIT_REPOSITORY "https://github.com/marzer/tomlplusplus"
  GIT_TAG "v3.4.0"
  GIT_SHALLOW ON
  GIT_PROGRESS ON
)

# Import
set(BUILD_TESTING OFF)
set(BUILD_EXAMPLES OFF)
set(BUILD_FUZZER OFF)
add_subdirectory(
  ${tomlplusplus_SOURCE_DIR}
  EXCLUDE_FROM_ALL
  SYSTEM
)
if(NOT TARGET tomlplusplus_tomlplusplus OR NOT TARGET tomlplusplus::tomlplusplus)
    message(FATAL_ERROR "tomlplusplus::tomlplusplus target is missing")
endif()
set_target_properties(tomlplusplus_tomlplusplus PROPERTIES FOLDER external)

list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "done")
