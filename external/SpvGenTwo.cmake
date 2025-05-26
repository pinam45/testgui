#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#

# Fix for CMake pre 3.20 behaviour, see CMP0118
set_source_files_properties(${global_generated_files_list} PROPERTIES GENERATED ON)

if(TARGET SpvGenTwo::SpvGenTwoLib)
    return()
endif()

message(CHECK_START "external: configuring SpvGenTwo")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

# Download
FetchContent_Populate(
  spvgentwo
  GIT_REPOSITORY "https://github.com/rAzoR8/SpvGenTwo"
  GIT_TAG "4bdddca967fb01311076c67ad8dce9a6d6ef7730"
  GIT_SHALLOW ON
  GIT_PROGRESS ON
)

# Import
set(BUILD_TESTING OFF)
set(SPVGENTWO_BUILD_TESTS OFF)
set(SPVGENTWO_ENABLE_WARNINGS OFF)
set(SPVGENTWO_LOGGING OFF)
set(SPVGENTWO_DEBUG_HEAP_ALLOC OFF)
add_subdirectory(
  ${spvgentwo_SOURCE_DIR}
  EXCLUDE_FROM_ALL
  SYSTEM
)
if(NOT TARGET SpvGenTwoLib)
    message(FATAL_ERROR "SpvGenTwoLib target is missing")
endif()
set_target_properties(SpvGenTwoLib PROPERTIES FOLDER external)
add_library(SpvGenTwo::SpvGenTwoLib ALIAS SpvGenTwoLib)

list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "done")
