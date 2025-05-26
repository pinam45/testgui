# apply_patch.cmake
# Usage: cmake -D PATCH_FILE=path/to/patch -DSOURCE_DIR=path/to/source -P apply_patch.cmake

if(NOT PATCH_FILE)
    message(FATAL_ERROR "PATCH_FILE must be provided")
endif()
if(NOT SOURCE_DIR)
    message(FATAL_ERROR "SOURCE_DIR must be provided")
endif()

if(NOT EXISTS "${PATCH_FILE}")
    message(FATAL_ERROR "Patch file does not exist: ${PATCH_FILE}")
endif()

# Store marker file in SOURCE directory so it gets cleaned when GIT_TAG changes
set(PATCH_APPLIED_FILE "${SOURCE_DIR}/.cmake_patch_applied")

if(NOT EXISTS "${PATCH_APPLIED_FILE}")
    find_program(GIT_EXECUTABLE git)
    if(NOT GIT_EXECUTABLE)
        message(FATAL_ERROR "Git not found")
    endif()

    message(STATUS "Applying patch...")
    execute_process(
      COMMAND ${GIT_EXECUTABLE} apply --whitespace=fix "${PATCH_FILE}"
      WORKING_DIRECTORY "${SOURCE_DIR}"
      RESULT_VARIABLE PATCH_RESULT
      OUTPUT_VARIABLE PATCH_OUTPUT
      ERROR_VARIABLE PATCH_ERROR
    )

    if(PATCH_RESULT EQUAL 0)
        file(WRITE "${PATCH_APPLIED_FILE}" "")
        message(STATUS "Patch applied successfully")
    else()
        message(STATUS "Patch output: ${PATCH_OUTPUT}")
        message(STATUS "Patch error: ${PATCH_ERROR}")
        message(FATAL_ERROR "Failed to apply patch")
    endif()
else()
    message(STATUS "Patch already applied, skipping")
endif()
