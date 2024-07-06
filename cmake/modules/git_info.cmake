#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#
message(CHECK_START "Git information target generation")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

# Set variables
get_filename_component(git_info_target_dir "${CMAKE_CURRENT_BINARY_DIR}/git_info" ABSOLUTE)
get_filename_component(top_project_dir "${PROJECT_SOURCE_DIR}" ABSOLUTE)
set(git_info_source_dir "${CMAKE_CURRENT_LIST_DIR}/git_info")
set(pre_hpp "${git_info_source_dir}/git_info.hpp.in")
set(post_hpp "${git_info_target_dir}/include/git_info.hpp")
set(git_state_file "${git_info_target_dir}/git-state.txt")
set(update_script_file "${git_info_source_dir}/update_git_info.cmake")

# Create target directories
set(directories "${git_info_target_dir}" "${git_info_target_dir}/include")
foreach(directory ${directories})
    if(NOT EXISTS "${directory}")
        file(MAKE_DIRECTORY "${directory}")
    endif()
endforeach()

find_package(Git QUIET)
if(NOT GIT_FOUND)
    message(WARNING "git not found, git information will be invalid")

    # Create header
    set(GIT_RETRIEVED_STATE "false")
    set(GIT_HEAD_BRANCH "unknown")
    set(GIT_HEAD_SHA1 "unknown")
    set(GIT_HEAD_SHA1_SHORT "unknown")
    set(GIT_IS_DIRTY "true")
    configure_file("${PRE_CPP}" "${POST_CPP}" @ONLY)
else()
    message(STATUS "git found: ${GIT_EXECUTABLE}")

    # Update command
    set(update_git_info_command
      ${CMAKE_COMMAND}
      -DWORKING_DIRECTORY=${top_project_dir}
      -DGIT_EXECUTABLE=${GIT_EXECUTABLE}
      -DGIT_STATE_FILE=${git_state_file}
      -DPRE_HPP=${pre_hpp}
      -DPOST_HPP=${post_hpp}
      -P "${update_script_file}"
    )

    # Create header
    execute_process(
      COMMAND ${update_git_info_command}
      COMMAND_ERROR_IS_FATAL ANY
    )

    # Create update target
    add_custom_target(
      update_git_info
      ALL
      DEPENDS "${pre_hpp}"
      BYPRODUCTS "${post_hpp}"
      COMMENT "Update git repository information for git_info"
      COMMAND ${update_git_info_command}
    )
    set_target_properties(update_git_info PROPERTIES FOLDER git)
endif()

# Create target
add_library(git_info INTERFACE)
target_include_directories(git_info SYSTEM INTERFACE "${git_info_target_dir}/include")
target_compile_features(git_info INTERFACE cxx_std_17)
set_target_properties(git_info PROPERTIES FOLDER git)
if(GIT_FOUND)
    add_dependencies(git_info update_git_info)
endif()

list(POP_BACK CMAKE_MESSAGE_INDENT)
message(CHECK_PASS "done")
