#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#

# Fix for CMake pre 3.20 behaviour, see CMP0118
set_source_files_properties(${global_generated_files_list} PROPERTIES GENERATED ON)

# Patch file path
file(TO_CMAKE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/fix.patch" PATCH_FILE)

# Download ImGuiColorTextEdit
FetchContent_Populate(
  imguicolortextedit
  GIT_REPOSITORY "https://github.com/goossens/ImGuiColorTextEdit"
  GIT_TAG "81afc72cbe1b18384c0fe09711956d52e0c49fd4"
  GIT_SHALLOW ON
  GIT_PROGRESS ON
  PATCH_COMMAND ${CMAKE_COMMAND}
  -D PATCH_FILE=${PATCH_FILE}
  -D SOURCE_DIR=<SOURCE_DIR>
  -P ${CMAKE_SOURCE_DIR}/cmake/apply_patch.cmake
)

# Declare ImGuiColorTextEdit
add_library(ImGuiColorTextEdit STATIC)
add_library(ImGuiColorTextEdit::ImGuiColorTextEdit ALIAS ImGuiColorTextEdit)

# Add sources
file(
  GLOB sources
  "${imguicolortextedit_SOURCE_DIR}/*.cpp"
  "${imguicolortextedit_SOURCE_DIR}/*.h"
)
target_sources(ImGuiColorTextEdit PRIVATE ${sources})

# Add includes
target_include_directories(
  ImGuiColorTextEdit SYSTEM PUBLIC
  "${imguicolortextedit_SOURCE_DIR}"
)

# Add dependencies
target_link_libraries(
  ImGuiColorTextEdit PUBLIC
  imgui::imgui
)

# Configure
target_compile_features(ImGuiColorTextEdit PRIVATE cxx_std_17)
set_target_properties(ImGuiColorTextEdit PROPERTIES FOLDER external)
