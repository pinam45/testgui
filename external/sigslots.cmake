#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#

# Fix for CMake pre 3.20 behaviour, see CMP0118
set_source_files_properties(${global_generated_files_list} PROPERTIES GENERATED ON)

if(TARGET Pal::Sigslot)
    return()
endif()

message(CHECK_START "external: configuring sigslot")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

# Download
FetchContent_Populate(
  sigslot
  GIT_REPOSITORY "https://github.com/palacaze/sigslot"
  GIT_TAG "v1.2.2"
  GIT_SHALLOW ON
  GIT_PROGRESS ON
)

# Import
set(BUILD_TESTING OFF)
set(SIGSLOT_COMPILE_TESTS OFF CACHE INTERNAL "" FORCE)
set(SIGSLOT_REDUCE_COMPILE_TIME OFF CACHE INTERNAL "" FORCE)
set(SIGSLOT_ENABLE_INSTALL OFF CACHE INTERNAL "" FORCE)
add_subdirectory(
  ${sigslot_SOURCE_DIR}
  EXCLUDE_FROM_ALL
  SYSTEM
)
if(NOT TARGET sigslot OR NOT TARGET Pal::Sigslot)
    message(FATAL_ERROR "Pal::Sigslot target is missing")
endif()
set_target_properties(sigslot PROPERTIES FOLDER external)

list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "done")
