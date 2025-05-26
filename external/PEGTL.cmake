#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#

# Fix for CMake pre 3.20 behaviour, see CMP0118
set_source_files_properties(${global_generated_files_list} PROPERTIES GENERATED ON)

if(TARGET taocpp::pegtl)
    return()
endif()

message(CHECK_START "external: configuring pegtl")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

# Download
FetchContent_Populate(
  pegtl
  GIT_REPOSITORY "https://github.com/taocpp/PEGTL"
  GIT_TAG "3.2.7"
  GIT_SHALLOW ON
  GIT_PROGRESS ON
)

# Import
set(BUILD_TESTING OFF)
set(PEGTL_BUILD_TESTS OFF CACHE INTERNAL "" FORCE)
set(PEGTL_BUILD_EXAMPLES OFF CACHE INTERNAL "" FORCE)
add_subdirectory(
  ${pegtl_SOURCE_DIR}
  EXCLUDE_FROM_ALL
  SYSTEM
)
if(NOT TARGET pegtl OR NOT TARGET taocpp::pegtl)
    message(FATAL_ERROR "taocpp::pegtl target is missing")
endif()
set_target_properties(pegtl PROPERTIES FOLDER external)

list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "done")
