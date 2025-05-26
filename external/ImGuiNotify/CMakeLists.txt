#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#

# Fix for CMake pre 3.20 behaviour, see CMP0118
set_source_files_properties(${global_generated_files_list} PROPERTIES GENERATED ON)

# Download ImGuiNotify
FetchContent_Populate(
  imguinotify
  GIT_REPOSITORY "https://github.com/TyomaVader/ImGuiNotify"
  GIT_TAG "aee35595d9b91a47ca4e02427641b50f307b47bf"
  #GIT_SHALLOW ON
  GIT_PROGRESS ON
)

# Move header to dedicated folder
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include")
configure_file(
  "${imguinotify_SOURCE_DIR}/unixExample/backends/ImGuiNotify.hpp"
  "${CMAKE_CURRENT_BINARY_DIR}/include"
  COPYONLY
)

# Declare ImGuiNotify
add_library(ImGuiNotify INTERFACE)
add_library(ImGuiNotify::ImGuiNotify ALIAS ImGuiNotify)

# Add includes
target_include_directories(
  ImGuiNotify SYSTEM INTERFACE
  "${CMAKE_CURRENT_BINARY_DIR}/include"
)

# Add dependencies
target_link_libraries(
  ImGuiNotify INTERFACE
  imgui::imgui
)

# Configure
target_compile_features(ImGuiNotify INTERFACE cxx_std_17)
set_target_properties(ImGuiNotify PROPERTIES FOLDER external)
