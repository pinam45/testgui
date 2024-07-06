#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#

# Required variables:
# WORKING_DIRECTORY
# GIT_EXECUTABLE
# GIT_STATE_FILE
# PRE_HPP
# POST_HPP

# Check variables
foreach(var IN ITEMS WORKING_DIRECTORY GIT_EXECUTABLE GIT_STATE_FILE PRE_HPP POST_HPP)
    if(NOT DEFINED ${var})
        message(FATAL_ERROR "Missing variable ${var}")
        return()
    endif()
endforeach()

# Paths to absolute
foreach(var IN ITEMS WORKING_DIRECTORY GIT_EXECUTABLE GIT_STATE_FILE PRE_HPP POST_HPP)
    get_filename_component(${var} "${${var}}" ABSOLUTE)
endforeach()

# Setup retrieved_state
set(retrieved_state "true")

# Get head branch
execute_process(COMMAND
  "${GIT_EXECUTABLE}" rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY "${WORKING_DIRECTORY}"
  RESULT_VARIABLE result
  OUTPUT_VARIABLE head_branch
  ERROR_QUIET
  OUTPUT_STRIP_TRAILING_WHITESPACE)
if(NOT result EQUAL 0)
    set(retrieved_state "false")
    set(head_branch "unknown")
endif()

# Get head sha1 (full)
execute_process(COMMAND
  "${GIT_EXECUTABLE}" rev-parse --verify HEAD
  WORKING_DIRECTORY "${WORKING_DIRECTORY}"
  RESULT_VARIABLE result
  OUTPUT_VARIABLE head_sha1
  ERROR_QUIET
  OUTPUT_STRIP_TRAILING_WHITESPACE)
if(NOT result EQUAL 0)
    set(retrieved_state "false")
    set(head_sha1 "unknown")
endif()

# Get head sha1 (short)
execute_process(COMMAND
  "${GIT_EXECUTABLE}" rev-parse --short --verify HEAD
  WORKING_DIRECTORY "${WORKING_DIRECTORY}"
  RESULT_VARIABLE result
  OUTPUT_VARIABLE head_sha1_short
  ERROR_QUIET
  OUTPUT_STRIP_TRAILING_WHITESPACE)
if(NOT result EQUAL 0)
    set(retrieved_state "false")
    set(head_sha1_short "unknown")
endif()

# Get whether or not the working tree is dirty
execute_process(COMMAND
  "${GIT_EXECUTABLE}" status --porcelain
  WORKING_DIRECTORY "${WORKING_DIRECTORY}"
  RESULT_VARIABLE result
  OUTPUT_VARIABLE out
  ERROR_QUIET
  OUTPUT_STRIP_TRAILING_WHITESPACE)
if(NOT result EQUAL 0)
    set(retrieved_state "false")
    set(is_dirty "false")
else()
    if(NOT "${out}" STREQUAL "")
        set(is_dirty "true")
    else()
        set(is_dirty "false")
    endif()
endif()

# Check if the state has changed from save
set(state ${retrieved_state} ${head_branch} ${head_sha1} ${head_sha1_short} ${is_dirty})
set(state_changed ON)
if(EXISTS "${GIT_STATE_FILE}")
    file(READ "${GIT_STATE_FILE}" old_state)
    if(old_state STREQUAL "${state}")
        set(state_changed OFF)
    endif()
endif()

# Update state save
if(state_changed OR NOT EXISTS "${POST_HPP}")
    file(WRITE "${GIT_STATE_FILE}" "${state}")
    set(GIT_RETRIEVED_STATE "${retrieved_state}")
    set(GIT_HEAD_BRANCH "${head_branch}")
    set(GIT_HEAD_SHA1 "${head_sha1}")
    set(GIT_HEAD_SHA1_SHORT "${head_sha1_short}")
    set(GIT_IS_DIRTY "${is_dirty}")
    configure_file("${PRE_HPP}" "${POST_HPP}" @ONLY)
endif()
