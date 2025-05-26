#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#

# Fix for CMake pre 3.20 behaviour, see CMP0118
set_source_files_properties(${global_generated_files_list} PROPERTIES GENERATED ON)

if(TARGET utf8cpp::utf8cpp)
    return()
endif()

message(CHECK_START "external: configuring utfcpp")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

# Download
FetchContent_Populate(
  utfcpp
  GIT_REPOSITORY "https://github.com/nemtrif/utfcpp"
  GIT_TAG "v4.0.5"
  GIT_SHALLOW ON
  GIT_PROGRESS ON
)

# Import
set(BUILD_TESTING OFF)
add_subdirectory(
  ${utfcpp_SOURCE_DIR}
  EXCLUDE_FROM_ALL
  SYSTEM
)
if(NOT TARGET utf8cpp)
    message(FATAL_ERROR "utf8cpp target is missing")
endif()
set_target_properties(utf8cpp PROPERTIES FOLDER external)
add_library(utf8cpp::utf8cpp ALIAS utf8cpp)

list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "done")
