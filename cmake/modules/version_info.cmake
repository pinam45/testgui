#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#
message(CHECK_START "Version information target generation")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

# Set variables
get_filename_component(version_info_target_dir "${CMAKE_CURRENT_BINARY_DIR}/version_info" ABSOLUTE)
set(version_info_source_dir "${CMAKE_CURRENT_LIST_DIR}/version_info")
set(pre_hpp "${version_info_source_dir}/version_info.hpp.in")
set(post_hpp "${version_info_target_dir}/include/version_info.hpp")

# Create target directories
set(directories "${version_info_target_dir}" "${version_info_target_dir}/include")
foreach(directory ${directories})
    if(NOT EXISTS "${directory}")
        file(MAKE_DIRECTORY "${directory}")
    endif()
endforeach()

# Create header
configure_file("${pre_hpp}" "${post_hpp}" @ONLY)

# Create target
add_library(version_info INTERFACE)
target_include_directories(version_info SYSTEM INTERFACE "${version_info_target_dir}/include")
target_compile_features(version_info INTERFACE cxx_std_17)
set_target_properties(version_info PROPERTIES FOLDER version)

list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "done")
