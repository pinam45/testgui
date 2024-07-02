#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#

# Fix for CMake pre 3.20 behaviour, see CMP0118
set_source_files_properties(${global_generated_files_list} PROPERTIES GENERATED ON)

if(TARGET spdlog::spdlog)
    return()
endif()

message(CHECK_START "external: configuring spdlog")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

# Dependencies
include(${CMAKE_CURRENT_SOURCE_DIR}/external/fmt.cmake)

# Download
FetchContent_Populate(
  spdlog
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/spdlog-src"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/spdlog-build"
  SUBBUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/spdlog-subbuild"
  GIT_REPOSITORY "https://github.com/gabime/spdlog"
  GIT_TAG "v1.11.0"
  GIT_SHALLOW ON
  GIT_PROGRESS ON
)

# Import
set(BUILD_TESTING OFF)
set(SPDLOG_FMT_EXTERNAL ON)
add_subdirectory(
  ${spdlog_SOURCE_DIR}
  EXCLUDE_FROM_ALL
  SYSTEM
)
if(NOT TARGET spdlog::spdlog)
    message(FATAL_ERROR "spdlog target is missing")
endif()

list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "done")
