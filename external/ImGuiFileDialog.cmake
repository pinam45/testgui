#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#

# Fix for CMake pre 3.20 behaviour, see CMP0118
set_source_files_properties(${global_generated_files_list} PROPERTIES GENERATED ON)

if(TARGET ImGuiFileDialog::ImGuiFileDialog)
    return()
endif()

message(CHECK_START "external: configuring ImGuiFileDialog")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

# Dependencies
include(${CMAKE_CURRENT_SOURCE_DIR}/external/imgui.cmake)

# Download
FetchContent_Populate(
  imguifiledialog
  GIT_REPOSITORY "https://github.com/aiekick/ImGuiFileDialog"
  GIT_TAG "cd5d354cbfdda389422494fa47fced59ad4d25a7"
  GIT_SHALLOW ON
  GIT_PROGRESS ON
)

# Import
add_subdirectory(
  ${imguifiledialog_SOURCE_DIR}
  EXCLUDE_FROM_ALL
  SYSTEM
)
if(NOT TARGET ImGuiFileDialog)
    message(FATAL_ERROR "ImGuiFileDialog target is missing")
endif()
set_target_properties(ImGuiFileDialog PROPERTIES FOLDER external)
target_link_libraries(ImGuiFileDialog PRIVATE imgui::imgui)
add_library(ImGuiFileDialog::ImGuiFileDialog ALIAS ImGuiFileDialog)

list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "done")
