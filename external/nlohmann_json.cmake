#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#

# Fix for CMake pre 3.20 behaviour, see CMP0118
set_source_files_properties(${global_generated_files_list} PROPERTIES GENERATED ON)

if(TARGET nlohmann_json::nlohmann_json)
    return()
endif()

message(CHECK_START "external: configuring nlohmann_json")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

# Download
FetchContent_Populate(
  nlohmann_json
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/nlohmann_json-src"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/nlohmann_json-build"
  SUBBUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/nlohmann_json-subbuild"
  GIT_REPOSITORY "https://github.com/nlohmann/json"
  GIT_TAG "v3.11.3"
  GIT_SHALLOW ON
  GIT_PROGRESS ON
)

# Import
set(BUILD_TESTING OFF)
set(JSON_BuildTests OFF)
set(JSON_Install OFF)
add_subdirectory(
  ${nlohmann_json_SOURCE_DIR}
  EXCLUDE_FROM_ALL
  SYSTEM
)
if(NOT TARGET nlohmann_json::nlohmann_json)
    message(FATAL_ERROR "nlohmann_json::nlohmann_json target is missing")
endif()

# Disable warnings on headers
get_target_property(nlohmann_json_include_directories nlohmann_json INTERFACE_INCLUDE_DIRECTORIES)
set_target_properties(nlohmann_json PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "")
target_include_directories(nlohmann_json SYSTEM INTERFACE ${nlohmann_json_include_directories})

list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "done")
