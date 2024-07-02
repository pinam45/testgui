#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#

# Fix for CMake pre 3.20 behaviour, see CMP0118
set_source_files_properties(${global_generated_files_list} PROPERTIES GENERATED ON)

if(TARGET imspinner::imspinner)
    return()
endif()

message(CHECK_START "external: configuring imspinner")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

# Dependencies
include(${CMAKE_CURRENT_SOURCE_DIR}/external/imgui.cmake)

# Custom CMake project
add_subdirectory(
  "${CMAKE_CURRENT_LIST_DIR}/imspinner"
  "${CMAKE_CURRENT_BINARY_DIR}/_deps/imspinner-build"
  EXCLUDE_FROM_ALL
)

list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "done")
