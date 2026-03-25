cmake_minimum_required(VERSION 3.15)

project(system_theme_pp_examples)

macro(add_example EXAMPLE_NAME EXAMPLE_DIR)
    file(GLOB_RECURSE EXAMPLE_SOURCES
        "${EXAMPLE_DIR}/*.cpp"
        "${EXAMPLE_DIR}/*.hpp"
        "${EXAMPLE_DIR}/*.c"
        "${EXAMPLE_DIR}/*.h"
    )

    message(STATUS "Building example ${EXAMPLE_NAME}")

    add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCES})

    target_link_libraries(${EXAMPLE_NAME} PRIVATE system_theme_pp)

    # Copy DLL next to the executable after build
    add_custom_command(TARGET ${EXAMPLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:system_theme_pp>
            $<TARGET_FILE_DIR:${EXAMPLE_NAME}>
    )
endmacro()

add_example(terminal ${CMAKE_CURRENT_LIST_DIR}/terminal)