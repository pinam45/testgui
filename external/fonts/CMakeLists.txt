#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#

# Fix for CMake pre 3.20 behaviour, see CMP0118
set_source_files_properties(${global_generated_files_list} PROPERTIES GENERATED ON)

# Declare fonts
add_library(fonts STATIC)
add_library(fonts::fonts ALIAS fonts)

# Add sources
file(
    GLOB sources
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)
target_sources(fonts PRIVATE ${sources})

# Add includes
target_include_directories(
    fonts SYSTEM PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

# Configure
target_compile_features(fonts PRIVATE cxx_std_17)
set_target_properties(fonts PROPERTIES FOLDER external)
