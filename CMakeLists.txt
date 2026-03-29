cmake_minimum_required(VERSION 3.15)

project(system_theme_pp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

string(REGEX MATCH "^Visual Studio" VS_GENERATOR_MATCH "${CMAKE_GENERATOR}")

if(VS_GENERATOR_MATCH MATCHES "Visual Studio")
    set(IS_VS_GENERATOR TRUE)
else()
    set(IS_VS_GENERATOR FALSE)
endif()


add_subdirectory(lib/gnome-css)

message("Generator: ${IS_VS_GENERATOR}")
# Set default build type to Release for single-config generators.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")

    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Set default startup configuration to Release for multi-config generators.
if(CMAKE_CONFIGURATION_TYPES)
    if(NOT IS_VS_GENERATOR)
        set(CMAKE_DEFAULT_BUILD_TYPE Release)
    endif()
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release;MinSizeRel;RelWithDebInfo" CACHE STRING "" FORCE)
endif()

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
    target_link_libraries(system_theme_pp PRIVATE UxTheme WindowsApp Microsoft::CppWinRT)
    target_compile_options(system_theme_pp PRIVATE /await)
elseif(UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(FONTCONFIG REQUIRED fontconfig)
    pkg_check_modules(GIO REQUIRED gio-2.0)
    find_package(DBus1 REQUIRED)
    target_link_libraries(system_theme_pp PRIVATE dbus-1 gnome-css)

    target_include_directories(system_theme_pp PRIVATE 
        ${CMAKE_CURRENT_LIST_DIR}/src/linux 
        ${FONTCONFIG_INCLUDE_DIRS} 
        ${GIO_INCLUDE_DIRS}
    )
    target_link_libraries(system_theme_pp PRIVATE 
        ${FONTCONFIG_LIBRARIES} 
        ${GIO_LIBRARIES}
    )
elseif(APPLE)
    target_include_directories(system_theme_pp PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/macos)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()