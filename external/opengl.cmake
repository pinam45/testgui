#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#

# Fix for CMake pre 3.20 behaviour, see CMP0118
set_source_files_properties(${global_generated_files_list} PROPERTIES GENERATED ON)

if(TARGET OpenGL::GL)
	return()
endif()

message(CHECK_START "external: configuring OpenGL")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
	message(FATAL_ERROR "OpenGL not found")
endif()
if(NOT TARGET OpenGL::GL)
	message(FATAL_ERROR "OpenGL::GL target is missing")
endif()

list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "done")
