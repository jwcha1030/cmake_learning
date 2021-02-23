# CMake Notes

## I. Introduction
CMake is cross-platform free and open-source software tool for build automation, testing and packaging via a compiler-independent method. CMake does not build the project, it generates the files needed by your build tool (e.g., GNU make, Visual Studio, etc.). A key feature is the ability to place compiler outputs (e.g., object files) outside the source tree, which enables multiple builds from the same source tree and cross-compilation. This ability keeps the source tree clean and removing a build directory to not remove the source files.

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
`CMakeLists.txt` in either Directories or Scripts may use the `include()` command to load a `<module>.cmake` source file in the scope of the including context.

Example:
```cmake
if(CUSTOM_LIB_DIR)
  include(misc/cmake/common/libbuild.cmake)
```

## III. User Interface

### **D. CMake Cache**
For CMake to execute, it needs to find the locations of compilers, tools and dependencies. Moreover, it needs to be able to conistently re-generate a buildsystem to use the same compile/link flags and paths to dependencies. These parameters are configurable by the users as they are paths and options specific to the user's system.

The cache entries may have a type to signify how it should be presented to the user interface (e.g., `cmake-gui` or `ccmake`)

The CMake files shipped with a software package may also define boolean toggle options using `option()` command. The command creates a cache entry which has a help text and a default value as shown below:

>`CMakeLists.txt`
```cmake
option(USE_MYMATH "Use tutorial provided math implementation" ON)
```
</br>

>`CMakeCache.txt`
```
//Use tutorial provided math implementation
USE_MYMATH:BOOL=ON
```

Such cache entries are typically specific to the provided software and affect the configuration of the build.

### **E. Presets**
CMake reads a file, `CMakePresets.json` for saving presets for commonly-used configure settings. These presets can set the build directory, generator, cache variables, environment variables, and other command-line options. For more information read <a href="https://cmake.org/cmake/help/v3.20/manual/cmake-presets.7.html#manual:cmake-presets(7)">cmake-presets</a> manual.

The following is an example of `CMakePresets.json`:
```json
{
  "version": 1,
  "configurePresets": [
    {
      "name": "ninja-release",
      "binaryDir": "${sourceDir}/build/${presetName}",
      "generator": "Ninja",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Release"
      }
    }
  ]
}
```
To see the list of available presets, run:
```
$ cmake -S /path/to/source --list-presets
```

### **F. Build**
After generating the buildsystem (i.e., via `cmake`), the software can be built by invoking the particular build tool. The build tool can be IDE generators or else CMake is aware of the specific build tool needed to invoke a build. The following command can be used in the build directory:

```
$ cmake --build ./
```
The `--build` flag invokes `CMAKE_MAKE_PROGRAM` command associated with the generator, or the build tool configured by the user.

>CMakeCache.txt
```
CMAKE_MAKE_PROGRAM: FILEPATH=/usr/bin/make
```

The `--build` mode also accepts the parameter `--target` or `-t` to specify a particular target(s) to build. Multiple targets may be given, separated by spaces.

```
$ cmake --build ./ --target myexe
```

USee with `--verbose` flag to print details of command lines invoked during the build.

### **G. Selecting a Target**
A build target is described executable and library in the CMake files. A target can also be the name of an action to carry out, such as `clean` (i.e., <a href="https://www.gnu.org/software/make/manual/html_node/Phony-Targets.html#Phony-Targets">Phony Targets</a>). CMake provides some built-in targets for all buildsystems providing in CMake files
- `all` : The default target used by `Makefile` and `Ninja` generators. Builds all targets in the buildsystem, except those excluded by `EXCLUDE_FROM_ALL` target property or `EXCLUDE_FROM_ALL` directory property.
- `clean` : Delete the built oject files and other output files.
- `test` : Runs tests. This target is only automatically available if the CMake files provide CTest-based tests.
- `install` : Installs software. This is available if the software defines install rues with the `install()` command.
- `package` : Creates a binary package. This is automatically available if the CMake files provide CPack-based packages.

For Makefile based systems, `/fast` variants of binary build targets are provided. The `/fast` variants are used to build the specified target without regard for its dependencies.

### **H. Software Installation**
The `CMAKE_INSTALL_PREFIX` variable can be set in the CMake cache to specify where to install the provided software. If the provided software has install rules specified using the `install()` command, they will install artificats into that prefix. On windows, the default installation location is to the `ProgramFiles` directory based on the computer's architecture. On Unix hosts, `/usr/local` is the default directory.

Some provided software specify `unintsall` rules, but CMake does not generate such by default.

### **I. Running Tests**
The `ctest` tool is comes with the CMake tools to execute provided tests and report the results. It allows fine control over which tests to run, how to run them, and how to report the results. Executing `ctest` in the build directory is equivalent to running the `test` target.

Examples:

A regular expression can be passed to run only tests which matches the expression. To run only tests with "Qt" in their name:
```
$ ctest -R Qt
```
To run tests without "Qt" in their name:
```
$ ctest -E Qt
```
Tests can also be run in parallel using given number of jobs:
```
$ ctest -R Qt -j8
```
The environment variable `CTEST_PARALLEL_LEVEL` can alternatively be set to avoid the need to pass `-j`. The command line argument `-V` or `--verbose` prints the output from all tests. The `--output-on-failure` option prints the test output for failing test only, which can also be set to 1 in the environment variable `CTEST_OUTPUT_ON_FAILURE` as an alternative to passing the option.
## IV. Starting Point
Create a `CMakeLists.txt` file in the `Step1` directory that looks like:
```
cmake_minimum_required(VERSION 3.6)

# set the project name
project(Tutorial)

#add the executable
add_executable(Tutorial tutorial.cxx)
```

The source code for tutorial.cxx is provided in the `Step1` directory and can be used to compute the square root of a number.