cmake_minimum_required(VERSION 3.6)

# set the project name
project(Tutorial VERSION 1.0)

# specify the C++ standard (Must be above the call add_executable)
# This is the easiest way to enable support for a specific C++ standard in CMake
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#add the executable
add_executable(Tutorial tutorial.cxx)

# configure a header file to pass some of the CMake settings to the source code
configure_file(TutorialConfig.h.in TutorialConfig.h)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(Tutorial PUBLIC "${PROJECT_BINARY_DIR}")