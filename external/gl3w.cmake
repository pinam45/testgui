#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#

# Fix for CMake pre 3.20 behaviour, see CMP0118
set_source_files_properties(${global_generated_files_list} PROPERTIES GENERATED ON)

if(TARGET gl3w::gl3w)
    return()
endif()

message(CHECK_START "external: configuring gl3w")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

# Download
FetchContent_Populate(
  gl3w
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/gl3w-src"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/gl3w-build"
  SUBBUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/gl3w-subbuild"
  GIT_REPOSITORY "https://github.com/skaslev/gl3w"
  GIT_TAG "3a33275633ce4be433332dc776e6a5b3bdea6506"
  #GIT_SHALLOW ON
  GIT_PROGRESS ON
  PATCH_COMMAND git apply "${CMAKE_CURRENT_SOURCE_DIR}/external/gl3w/fix.patch" || git status #FIXME
  # https://gitlab.kitware.com/cmake/cmake/-/issues/21146
)

# Import
set(BUILD_TESTING OFF)
set(OpenGL_GL_PREFERENCE GLVND)
add_subdirectory(
  ${gl3w_SOURCE_DIR}
  EXCLUDE_FROM_ALL
  SYSTEM
)
if(NOT TARGET gl3w)
    message(FATAL_ERROR "gl3w target is missing")
endif()
add_library(gl3w::gl3w ALIAS gl3w)

# Fix for CMake pre 3.20 behaviour, see CMP0118
set(global_generated_files_list "${global_generated_files_list};${gl3w_BINARY_DIR}/src/gl3w.c" CACHE INTERNAL "" FORCE)


list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "done")
