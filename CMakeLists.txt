cmake_minimum_required(VERSION 3.15)

project(system_theme_pp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


file(GLOB_RECURSE PRIVATE_CPP_SOURCES "src/shared/*.cpp")
file(GLOB_RECURSE PRIVATE_HPP_HEADERS "src/shared/*.hpp")
file(GLOB_RECURSE PRIVATE_C_SOURCES "src/shared/*.c")
file(GLOB_RECURSE PRIVATE_H_HEADERS "src/shared/*.h")

if(WIN32)
    message(STATUS "Building for Windows")
    file(GLOB_RECURSE PLATFORM_CPP_SOURCES "src/windows/*.cpp")
    file(GLOB_RECURSE PLATFORM_HPP_HEADERS "src/windows/*.hpp")
    file(GLOB_RECURSE PLATFORM_C_SOURCES "src/windows/*.c")
    file(GLOB_RECURSE PLATFORM_H_HEADERS "src/windows/*.h")
elseif(UNIX AND NOT APPLE)
    message(STATUS "Building for Linux")
    file(GLOB_RECURSE PLATFORM_CPP_SOURCES "src/linux/*.cpp")
    file(GLOB_RECURSE PLATFORM_HPP_HEADERS "src/linux/*.hpp")
    file(GLOB_RECURSE PLATFORM_C_SOURCES "src/linux/*.c")
    file(GLOB_RECURSE PLATFORM_H_HEADERS "src/linux/*.h")
elseif(APPLE)
    message(FATAL_ERROR "MacOS is not supported yet")
    file(GLOB_RECURSE PLATFORM_CPP_SOURCES "src/macos/*.cpp")
    file(GLOB_RECURSE PLATFORM_HPP_HEADERS "src/macos/*.hpp")
    file(GLOB_RECURSE PLATFORM_C_SOURCES "src/macos/*.c")
    file(GLOB_RECURSE PLATFORM_H_HEADERS "src/macos/*.h")
endif()

option(SHARED_LIBRARY "Build as a shared library" ON)
option(BUILD_EXAMPLES "Build example applications" ON)

if(SHARED_LIBRARY)
    add_library(system_theme_pp SHARED ${PRIVATE_CPP_SOURCES} ${PRIVATE_HPP_HEADERS} ${PRIVATE_C_SOURCES} ${PRIVATE_H_HEADERS} ${PLATFORM_CPP_SOURCES} ${PLATFORM_HPP_HEADERS} ${PLATFORM_C_SOURCES} ${PLATFORM_H_HEADERS})
    # Copy DLL to the same folder as the executable after build
    set_target_properties(system_theme_pp PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    )
else()
    add_library(system_theme_pp STATIC ${PRIVATE_CPP_SOURCES} ${PRIVATE_HPP_HEADERS} ${PRIVATE_C_SOURCES} ${PRIVATE_H_HEADERS} ${PLATFORM_CPP_SOURCES} ${PLATFORM_HPP_HEADERS} ${PLATFORM_C_SOURCES} ${PLATFORM_H_HEADERS})
endif()

target_compile_definitions(system_theme_pp PRIVATE "SYSTEM_THEME_PP_EXPORTS")

target_include_directories(system_theme_pp PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(system_theme_pp PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/shared)


if(WIN32)
    find_package(cppwinrt CONFIG REQUIRED)
    target_include_directories(system_theme_pp PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/windows)
    target_link_libraries(system_theme_pp PRIVATE UxTheme dwmapi WindowsApp Microsoft::CppWinRT)
    target_compile_options(system_theme_pp PRIVATE /await)
elseif(UNIX AND NOT APPLE)
    target_include_directories(system_theme_pp PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/linux)
elseif(APPLE)
    target_include_directories(system_theme_pp PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/macos)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()