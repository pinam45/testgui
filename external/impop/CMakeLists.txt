#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#

# Fix for CMake pre 3.20 behaviour, see CMP0118
set_source_files_properties(${global_generated_files_list} PROPERTIES GENERATED ON)

# Download impop
FetchContent_Populate(
  impop
  GIT_REPOSITORY "https://github.com/kfish/impop"
  GIT_TAG "a09c12f43ed6484430f166d197ea34406fe1b72c"
  GIT_PROGRESS ON
)

# Declare impop
add_library(impop INTERFACE)
add_library(impop::impop ALIAS impop)

# Add includes
target_include_directories(
  impop SYSTEM INTERFACE
  "${impop_SOURCE_DIR}/include/"
)

# Add dependencies
target_link_libraries(
  impop INTERFACE
  imgui::imgui
  implot::implot
)

# Configure
target_compile_features(impop INTERFACE cxx_std_17)
set_target_properties(impop PROPERTIES FOLDER external)
