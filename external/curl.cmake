#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#

# Fix for CMake pre 3.20 behaviour, see CMP0118
set_source_files_properties(${global_generated_files_list} PROPERTIES GENERATED ON)

if(TARGET CURL::libcurl)
    return()
endif()

message(CHECK_START "external: configuring curl")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

# Download
FetchContent_Populate(
  curl
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/curl-src"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/curl-build"
  SUBBUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/curl-subbuild"
  GIT_REPOSITORY "https://github.com/curl/curl"
  GIT_TAG "curl-8_9_1"
  GIT_SHALLOW ON
  GIT_PROGRESS ON
  UPDATE_DISCONNECTED ON
)

# Import
set(BUILD_TESTING OFF)
set(BUILD_CURL_EXE OFF)
set(BUILD_SHARED_LIBS OFF)
set(HTTP_ONLY ON)
set(USE_LIBIDN2 OFF)
#if(WIN32)
#    set(USE_WIN32_IDN ON)
#endif()
set(CURL_DISABLE_INSTALL ON)
add_subdirectory(
  ${curl_SOURCE_DIR}
  EXCLUDE_FROM_ALL
  SYSTEM
)
if(NOT TARGET libcurl OR NOT TARGET CURL::libcurl)
    message(FATAL_ERROR "CURL::libcurl target is missing")
endif()
if(TARGET libcurl_object)
    set_target_properties(libcurl_object PROPERTIES FOLDER external)
endif()
set_target_properties(libcurl_static PROPERTIES FOLDER external)

list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "done")
