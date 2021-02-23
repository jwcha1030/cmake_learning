# CMake

## I. Introduction
CMake is cross-platform free and open-source software for build automation, testing and packaging via a compiler-independent method. A key feature is the ability to place compiler outputs (e.g., object files) outside the source tree, which enables multiple builds from the same source tree and cross-compilation. This ability keeps the source tree clean and removing a build directory to not remove the source files.

### **Flexible Project Structure**
CMake can locate system-wide and user-specified executables, files, and libraries. These locations are recorded in a cache (e.g., CMakeCache.txt) which can be customized before generating the target build files.

### **Build Process**
The build process of a program or library with CMake occur in two stages. First, standard build files are generated from configuration files (e.g., CMakeLists.txt) written in CMake language. Then, the platform's native build tools or toolchain are used for actual building of programs.

The build files are configured depending on used generator (e.g., `make` command in *Unix*). Generated files are typically placed (by using `cmake` flag) into a different from sources folder (e.g., build/).

## II. Organization
CMake input files are written in the "CMake Language" in source files named `CMakeLists.txt` or ending in a `.cmake` file name extension.

### **A. Directories**
The entry point(i.e., the top-level source directory) of a project source tree is a source file called `CMakeLists.txt`. This file contains build specification or uses `add_subdirectory()` command to add subdirectories to the build. Each subdirectory added by the command must conain a `CMakeLists.txt` file. The source directory which contains the cmake file act as the default working and output directory.

### **B. Scripts**
An individual `<script>.cmake` cource file may be processed in *script mode* by using <a href="https://cmake.org/cmake/help/latest/manual/cmake.1.html#manual:cmake(1)">cmake</a> command-line tool with the `-P` option. Script mode runs the commands in the given CMake Language source file and does not generate a build system.

### **C. Modules**
`CMakeLists.txt` in either Directories or Scripts may use the `include()` command to load a `<module>.cmake` source file 

## III. Starting Point
Create a `CMakeLists.txt` file in the `Step1` directory that looks like:
```
cmake_minimum_required(VERSION 3.6)

# set the project name
project(Tutorial)

#add the executable
add_executable(Tutorial tutorial.cxx)
```

The source code for tutorial.cxx is provided in the `Step1` directory and can be used to compute the square root of a number.