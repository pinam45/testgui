#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#

# Fix for CMake pre 3.20 behaviour, see CMP0118
set_source_files_properties(${global_generated_files_list} PROPERTIES GENERATED ON)

if(TARGET scope_guard::scope_guard)
    return()
endif()

message(CHECK_START "external: configuring scope_guard")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

# Download
FetchContent_Populate(
  scope_guard
  GIT_REPOSITORY "https://github.com/Neargye/scope_guard"
  GIT_TAG "v0.9.1"
  GIT_SHALLOW ON
  GIT_PROGRESS ON
)

# Import
set(BUILD_TESTING OFF)
set(SCOPE_GUARD_OPT_BUILD_EXAMPLES OFF)
set(SCOPE_GUARD_OPT_BUILD_TESTS OFF)
set(SCOPE_GUARD_OPT_INSTALL OFF)
add_subdirectory(
  ${scope_guard_SOURCE_DIR}
  EXCLUDE_FROM_ALL
  SYSTEM
)
if(NOT TARGET scope_guard OR NOT TARGET scope_guard::scope_guard)
    message(FATAL_ERROR "scope_guard::scope_guard target is missing")
endif()
set_target_properties(scope_guard PROPERTIES FOLDER external)

list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "done")
