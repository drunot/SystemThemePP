cmake_minimum_required(VERSION 3.10)
project(gnome-css C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(gnome-css STATIC ${CMAKE_CURRENT_LIST_DIR}/c_src/gnome_css.c ${CMAKE_CURRENT_LIST_DIR}/include/gnome_css/gnome_css.h)

# Custom target to build Rust static library
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_LIST_DIR}/target/release/libgnome_css.a
    COMMAND cargo build --release
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    COMMENT "Building Rust static library"
    VERBATIM
)

add_custom_target(
    rustlib ALL
    DEPENDS ${CMAKE_CURRENT_LIST_DIR}/target/release/libgnome_css.a
)

# Link the Rust static library to your C executable
add_dependencies(gnome-css rustlib)
target_link_libraries(gnome-css PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/target/release/libgnome_css.a
    m
)

target_include_directories(gnome-css PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)