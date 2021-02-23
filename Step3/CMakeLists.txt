cmake_minimum_required(VERSION 3.6)

# set the project name
project(Tutorial VERSION 2.0)

# specify the C++ standard (Must be above the call add_executable)
# This is the easiest way to enable support for a specific C++ standard in CMake
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


#STEP2: make the MathFunctions library optional. Add an option to the top-level of this file.
#This option will be displayed in the cmake-gui and ccmake with a default value of ON that can be changed by the user.
#This setting will be stored in the cache so that the user does not need to set the value each time they run CMake.
option(USE_MYMATH "Use tutorial provided math implementation" ON)

# configure a header file to pass some of the CMake settings to the source code
configure_file(TutorialConfig.h.in TutorialConfig.h)

#STEP2 Exercise: Why is it important that we configure Tutorial.h.in after the option for USE_MYMATH?
#What would happen if we inverted the two?
# -> "#cmakedefine USE_MYMATH" in Tutorial.h.in will not work, hence the source file (i.e., tutorial.cxx) 
# will not know what USE_MYMATH is.

#STEP2: make building and linking the MathFunctions library conditional.
#use of variable EXTRA_LIBS to collect up any optional libraries to later be linked into the executable.
#similar approach is taken for the variable EXTRA_INCLUDES for optional header files.
#this is a classic approach. A more modern approach will be covered in the next step.
if(USE_MYMATH)
  #add the MathFunctions library
  add_subdirectory(MathFunctions)
  list(APPEND EXTRA_LIBS MathFunctions)
  #STEP3: We have specified usage requirments for MathFunctions (in ./MathFunctions/CMakeLists.txt)
  # Hence, we can remove the line below from STEP2.
  # list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/MathFunctions")
endif()

#add the executable
add_executable(Tutorial tutorial.cxx)

#STEP2: add the new library to the executable
target_link_libraries(Tutorial PUBLIC "${EXTRA_LIBS}")

# add the binary tree to the search path for include files so that we will find TutorialConfig.h

#STEP3: we can remove include directory, as it was specified in ./MathFunctions/CMakeLists.txt
# target_include_directories(Tutorial PUBLIC "${PROJECT_BINARY_DIR}" "${EXTRA_INCLUDES}")
target_include_directories(Tutorial PUBLIC "${PROJECT_BINARY_DIR}")
