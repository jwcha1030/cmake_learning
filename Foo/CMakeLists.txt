cmake_minimum_required(VERSION 3.10)

# set(PROJ_NAME hello_world)
project(hello_world)
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB USER_DEFINED_FILES src/*.c src/*.cpp src/*.cxx src/*.cc)

# builds the target <app>
add_executable(app ${USER_DEFINED_FILES}) # Be sure to have only one main() function among the source files.

add_library(foo src/foo.cpp)
target_compile_features(foo PRIVATE cxx_constexpr)

# <type> for set() is "PATH". If the value of the <variable> (e.g., app_DATA) is a relative path, then the set command will treat the path as relative to the current
# working directory and convert it to an absolute path.
# FORCE overwrites CACHE, if FORCE does not exist the previous value is loaded even if set has been updated
set(PATH_DATA ${CMAKE_CURRENT_SOURCE_DIR}/data CACHE PATH "This directory contains all DATA and RESOURCES" FORCE)
set(THIRDPARTIES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/thirdparties CACHE PATH "This directory contains thirdparties" FORCE)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/common/config.h.in" "${CMAKE_CURRENT_SOURCE_DIR}/include/config.h")