#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#

# Fix for CMake pre 3.20 behaviour, see CMP0118
set_source_files_properties(${global_generated_files_list} PROPERTIES GENERATED ON)

if(TARGET utf8::cpp)
    return()
endif()

message(CHECK_START "external: configuring utfcpp")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

# Download
FetchContent_Populate(
  utfcpp
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/utfcpp-src"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/utfcpp-build"
  SUBBUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/utfcpp-subbuild"
  GIT_REPOSITORY "https://github.com/nemtrif/utfcpp"
  GIT_TAG "v3.2.2"
  GIT_SHALLOW ON
  GIT_PROGRESS ON
  UPDATE_DISCONNECTED ON
)

# Import
set(BUILD_TESTING OFF)
add_subdirectory(
  ${utfcpp_SOURCE_DIR}
  EXCLUDE_FROM_ALL
  SYSTEM
)
if(NOT TARGET utf8::cpp)
    message(FATAL_ERROR "utf8::cpp target is missing")
endif()

list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "done")
