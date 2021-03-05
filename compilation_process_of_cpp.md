# Understanding C++ from Source to Binaries
> Source : [Article](https://www.daniweb.com/programming/software-development/tutorials/466177/understanding-c-from-source-to-binaries)

This short guide covers some details of the compilation process from sources to binaries. It also features GNU tools (GCC) used in a Unix-like environment (POSIX). For Mac OSX, the preferred compiler is Clang++, which involves a tool-suite compatible with GNU tools.

## I. Terms
**Header File** : Contains C++ source code and they are mostly used to make *declarations* and/or templates. In other words, the files contain no function bodies, definitions of a variable, and etc. They only tell the compiler about the "things" that exists. Usual extensions: `.hpp`, `.h`, or `.hxx`.

**Source File** : Contains C++ source code and they are used exclusively used for *definitions*, i.e, the actual implementation of the "things" that make up the program or library. Usual extensions: `.cpp`, `.c`, or `.cxx` (normally `.c` is reserved for C source files, not C++).

**Translation Unit (TU)** : A technical term for a source file, once the compiler has looked at (i.e., after pre-processing) all `#include` statements and substituted them for the header files they refer to.

**Object File** : A binary file which is an output of the compilation of a translation unit into executable code. Usual extensions: `.o` or `.obj` (Microsoft Visual C++ Compiler, MSVC).

**Static Library** : A special static library used by the MVSC toolset to make the static link between a standalone executable and a dynamic-link library (DLL).

**Executable Program** : A type of binary file that can be started and executed, i.e., has an execution entry-point. This is a program you can run. Usual extension: `<nothing>` (POSIX) or `.exe` (Windows)

## II. Basic C++ Sources

The source code of C++ projects are composed of headers and source files. Both have distinct purposes. A header file is a file meant to be *included* in any source file that need to use the functions or templates declared, but not defined, in the file. It is no more than a set of declarations for functions and classes that can be used in any source file included that header.

Example: `my_simple_header.hpp`
```cpp
#ifndef MY_SIMPLE_HEADER_HPP
  #define MY_SIMPLE_HEADER_HPP

  double sin (double x);
  class Foo 
  {
    public:
      // .. with a data member:
      int bar;
      // .. with a static data member:
      static int value;

      // .. with a const member function:
      void hello() const;
      // .. with a static member function:
      static void world();
  };

  // declaration of a global variable:
  int global_var;
#endif
```

In the above example, there are many declarations of "things", such as functions, classes, data members, static data members, global variables, to name a few. The header file contains no implementations or definitions of such "things". Note that this is not a strict requirement, but a common practice.

*header-guard* is another important aspect of header files. It's a pre-processor condition (`#ifndef MY_SIMPLE_HEADER_HPP`) which guarantees that the header file's content appears only once in a given translation unit. In C++, *One Definition Rule* (ODR) states that "things" should only be defined once in the entire program and some "things", like classes, should only be declared once in a translation unit. When the pre-processor of a compiler sees a `#include` command, it finds the file in question and performs a copy-paste of tis content to where the `#include` appears. Without header-guards, the same content may appear several times, and even lead to an infinite recursion. Thus, **always use header-guards when writing a header file**. The name used in the header-guard (e.g., `MY_SIMPLE_HEADER_HPP`) is arbitrary but should be unique. By convention, it's often the name of the file itself in capital letters with the extension, and with a library-specific or project-specific prefix.

The source file, i.e., the cpp files contain the implementation of the "things" that were declared in an associated header file. Source files are compiled one by one, each becoming a separate object file once compiled. This is called the *separate compilation model* which is fundamental to C, C++, D, and most other compiled languages. In this compilation model, each source file must include every header file it requires to work. The compiler looks at one translation unit at a time for any code that is  declared in one of the included headers does not exist or not in the source file.

Source File Example: `foo.cpp`
```cpp
// include the necessary header files:
#include "my_simple_header.hpp"

// definition of the free function:
double sin(double x) {
  /* ... code goes here ... */
};

// definition of the static data member:
int Foo::value = 0;

// definition of the const member function:
void Foo::hello() const {
  /* ... code goes here ... */
};

// definition of the static member function:
void Foo::world() {
  /* ... code goes here ... */
};

// definition of the static member function:
int global_var = 0;
```
## III. Advanced C++ Sources
A strict, but not inviolable, rule is the ODR. Specifically, in the previous section, it was stated that headers should only contain declarations. Nevertheless, there are cases where functions are very small, like only a few lines or less, and seem tedious to declare the function in the header and then implement it in the source file. For a solution, one can simply make an *inline definition*; the definition can be put in the header file and mark the function with the keyword `inline`:

```cpp
#ifndef MY_SIMPLE_HEADER_HPP
  #DEFINE MY_SIMPLE_HEADER_HPP
  // declaration of an inline function
  inline double sin(double x){
    /*code */
  };
#endif
```
**Note**: Inline definitions do not mean the same as *inlined functions* (or *function inlining*). The former is a decision by the programmer about where to place the definition, and the latter is a decision by the compiler about how to best optimize the executable code. However, function inlining does require inline definitions.

Also, for functions within a class, `inline` keyword is not needed. Inline definitions can be written directly within the class declaration. On the other hand, functions outside the class but within the header file must include `inline` keyword:

```cpp
#ifndef MY_SIMPLE_HEADER_HPP
  #define MY_SIMPLE_HEADER_HPP
  //declaration of a class
  class Foo {
    public:
      void hello() const {
        /* .. inline code goes here .. */
      };

      void world();
  };

  inline void Foo::world() {
    /* code */
  };
#endif
```
Beyond the convenience, inline definitions of simple functions also make them candidates for function inlining, which is when the compiler optimizes the code by replacing function calls with the actual code of the function.

Finally, inline definitions are used to define a template in a header file. The subject of templates is beyond the scope of this guide, but suffice to say, a function template or a member function of a class template must be defined in the header file because they are not actual code but rather instructions to the compiler on how to generate code for a particular instance of the template, meaning that the definition must be available in all translation units in which the template is used.

## IV. How Does the Compiler Find Headers?

The compiler (pre-processor) maintains a list of *include-directories*, which is a list of all the directories it will consider and look for header files (by adding to it the relative path given in the include command). This is an ordered list (i.e., goes down the list, takes the first match). At the very least, that list has a number of system directories where one can typically find the standard headers or installed library headers. In Unix-like systems, the system directory is `/usr/include` and/or `/usr/local/include`.

You can instruct the compiler to consider additional include-directories by configuring your project configuration within your build script (e.g., makefile or using cmake), or simply as a command-line option when invoking the compiler manually (with option `-I`):

```
$ gcc -Wall -I /path/to/my_lib/include my_test_program.cpp -o my_test_program
```

In addition to the list of include-directories, the compiler can also consider the current directory, the directory in which the current source or header file is. To do so, you must use the double-quote notation for the include command, such as `#include "my_header.hpp"` instead of `#include <my_header.hpp>`. This is not strictly required by the standard.

## V. Linking
Linking outputs a runnable program. Consider the following simple source file (`hello.cpp`):
```cpp
#include "hello.hpp"
#include <iostream>

void hello() {
  std::cout << "Hello World!" << std::endl;
};
```
where `hello.hpp` is a header that simply declares the `hello()` function. We can compile it to obtain an object file as so:
```
$ g++ -Wall hello.cpp -c -o hello.o
```
which produces the object file called `hello.o`. The file contains binary code and organized into a number of sections in the file, each under a *symbol*. We can print out the list of symbols using this command:
```
$ nm hello.o
```
which prints:
```
                 U __cxa_atexit
                 U __dso_handle
000000000000005f t _GLOBAL__sub_I__Z5hellov
0000000000000022 t _Z41__static_initialization_and_destruction_0ii
0000000000000000 T _Z5hellov
                 U _ZNSolsEPFRSoS_E
                 U _ZNSt8ios_base4InitC1Ev
                 U _ZNSt8ios_base4InitD1Ev
                 U _ZSt4cout
                 U _ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_
0000000000000000 b _ZStL8__ioinit
                 U _ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
```
Without explaining everything thoroughly, the row `0000000000000000 b _ZStL8__ioinit` means that there is a function, called "hello", defined in this object file, at offset 0 (at the start). Then, I can point to all those `u` rows, which declare symbols that are "**U**sed" within the object file.

Say in this source file (`hello_main.cpp`):
```cpp
#include "hello.hpp"

int main(){
  hello();
  return 0;
};
```
is compiled to the object file `hello_main.o`:
```
$ g++ -Wall -c hello_main.cpp -o hello_main.o
$ nm hello_main.o
0000000000000000 T main
                 U _Z5hellov
```
As you can see, the object file has `main()` function and uses (or needs) `hello()` function. That symbol is defined in `hello.o` file, so putting them together can make the following program:
```
$ g++ hello_main.o hello.o -o hello_world
$ ./hello_world
Hello World!
```
By providing only object files to the `g++` compiler, it will see that it doesn't need to compile everything, and simply links object files together to produce the output `hello_world` program. A side note, you can use the linker program `ld` directly.

Hence, the linker's job is to take all the object files and libraries that it is given, and find a way to satisfy all the "U" (or "used") symbols by finding a definition for each of them. The way it achieves this job is by assembling a big list of all the defined symbols (a big hash-table), and then, goes through everything again to make the links that are needed.

## VI. Dynamic vs. Static Libraries

A *static library* is a collection of object files, grouped into a single file called an archive. GCC uses the `ar` program to create static libraries (like zipping files into a `.zip` file).

Here is the hello world example using a static library:
```
$ g++ -Wall hello.cpp -c -o hello.o
$ ar rcs libhello.a hello.o
$ g++ -Wall hello_main.cpp -o hello_world -L ./ -lhello
$ ./hello_world
Hello World!
```

The `-L` option tells the linker to add the current directory `./` to the list of directories to look for libraries to link with, and the `-lhello` tells the linker to link with the "hello" library; the linker will try to find a library file called `libhello.a` or `libhello.so` (dynamic). If a dynamic library exists, it will be preferred unless the `-static` option is used.

A *dynamic library* is a bit more complicated. The concept of the library is we want to collect a large amount of binary code into a single file and want to avoid having to bring all that code into the final executable programs. If many programs use the same library code, this can reduce the collective size of sum of all the executables. We can achieve this by having the executable program load and use the library as it runs, hence dynamically. The executable, thus, needs the dynamic library to run (i.e., it's a **run-time dependency**). Additionally, the executable needs to link with the dynamic library on its own, long after compilation called run-time or dynamic linking.

Dynamic loading and linking of the library involves few more processes than linking a static library. "Loading" means putting the code into RAM memory, loading global variables into the process address spaces, and running static-initialization code. To do so, a dynamic library needs all the same mechanics that an executable program has. Because the operating system handles the loading, a dynamic library must be packaged into a format similar to an executable (e.g., ELF or Executable and Linkable Format). For that reason, dynamic libraries are more like executable programs than they are like static libraries, in fact, technically-speaking, a dynamic library is just an executable without a "main" function.

Example using a dynamic library:
```
$ g++ -Wall hello.cpp -c -o hello.o
$ g++ -shared -fPIC -Wall hello.o -o libhello.so 
```

The above example creates a dynamic library (*shared object*) called `libhello.so`. The options `-shared` and `-fPIC` are necessary for creating a shared object. Now, we can compile the main program and link to that dynamic library:

```
$ g++ -Wall hello_main.cpp -L ./ -lhello -o hello_world
$ ./hello_world
./hello_world: error while loading shared libraries: libhello.so: cannot open shared object file: No such file or directory
```
The error shows up because the program cannot locate the library at which dynamic libraries are normally installed (e.g., `/lib`, `/usr/lib`, or `/usr/local/lib` for Unix; "system" and "system32" directories in Windows). Thus, one option is to add the current directory to the library paths environment variable in `.bashrc` or in the terminal (temporary use):
```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./
```

For a permanent solution, the library path can be embedded into the executable instead, to avoid modifying the user's environment variables:
```
$ g++ -Wall hello_main.cpp -o hello_world -Wl,-rpath,./ -L ./ -lhello
$ ./hello_world
Hello World!
```
The sequence `-Wl, -rpath,./` tells the linker to add the current directory (relative to where the program is) to the executable such that it will be considered first when looking from the dynamic library. For a more detailed explanation of options, refer to this [page](https://tldp.org/HOWTO/Program-Library-HOWTO/shared-libraries.html).

There some caveats to consider when using C++ dynamic libraries. Long story short, when you write shared-objects or dynamic-libraries, you must have a C interface (API), where all functions and types shared between the modules are in C. Check the end of the [article](https://www.daniweb.com/programming/software-development/tutorials/466177/understanding-c-from-source-to-binaries) for more details of the caveats.