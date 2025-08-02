# Tomurcuk

Collection of general-purpose utilities.

## Planned Features

- Memory allocators
- Array-based data structures
- Unicode strings
- Filesystem manipulation
- Bit manipulation
- Calendar arithmetic
- Hashing and comparisons
- Formatting
- Logging

## Coding Rules

- Error handling
  - No exceptions.
  - No run-time type information.
  - No destructors.
  - No operator overloading other than math.
  - Recoverable errors must be communicated via the API.
  - Irrecoverable errors must abort via a user-provided crash handler.
- Memory management
  - Functions that allocate must take an allocator as a parameter.
  - Allocators must not be stored in types.
  - All types must be trivially copyable.
- Logical structure
  - All functions and global variables must be inside a type.
  - All types must be inside the namespace `tomurcuk`.
  - All type names must be in `PascalCase`.
  - All function names must be in `camelCase`.
  - All local variable names must be in `camelCase`.
  - All global variable names must be in `PascalCase` with a `g` prefix.
  - All constants must be in `PascalCase` with a `k` prefix.
  - All enum variants must be in `PascalCase` with an `e` prefix.
  - All macros must be in `SCREAMING_SNAKE_CASE` with an `TOMURCUK_` prefix.
  - All public field names must be in `camelCase`.
  - All private field names must be in `PascalCase` with an `m` prefix.
- Physical structure
  - All types must be in their own file, which is named the same as the type.
  - All source files must be under the `sources/tomurcuk` directory.
  - All public header files must be under the `publicHeaders/tomurcuk`
    directory.
  - All private header files must be under the `privateHeaders/tomurcuk`
    directory.
  - All source files must have `.cpp` file extension.
  - All header files must have `.hpp` file extension.
- Format
  - All functions must be in trailing return type format.
  - All variables must be declared with `auto`.
  - All committed code must be formatted with `clang-format`.
  - Some code sections can be manually formatted to improve readability. This is
    usually good for code that can be formatted into a table by making some
    statements stay on the same line.
- Build system
  - Use latest possible version.
  - All `clang` extensions can be used.
  - All dependencies must be brought in via `FetchContent` of `CMake`.
  - All `CMake` target names must be in `PascalCase` with a `tomurcuk` prefix.
  - All `CMake` variable names must be in `SCREAMING_SNAKE_CASE` with an
    `TOMURCUK_` prefix.
