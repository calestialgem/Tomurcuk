# Coding Rules

## Error handling

- No exceptions.
- No run-time type information.
- No destructors.
- No operator overloading other than math.
- Recoverable errors must be communicated via the API.
- Irrecoverable errors must abort via a user-provided crash handler.

## Memory management

- Functions that allocate must take an allocator as a parameter.
- Allocators must not be stored in types.
- All types must be trivially copyable.

## Logical structure

- Code is divided into modules, packages, and types.
- All module and package names must be in `camelCase`.
- All type names must be in `PascalCase`.
- All types must be inside the namespace `<moduleName>`.
- All types must have a visibility out of `public` or `private`, which defines
  whether they can be accessed from the outside of the owning package.
- All functions and global variables must be inside a type.
- All function names must be in `camelCase`.
- All local variable names must be in `camelCase`.
- All public global variable (static field) names must be in `PascalCase` with a
  `g` prefix.
- All private global variable (static field) names must be in `PascalCase` with
  a `s` prefix.
- All constants must be in `PascalCase` with a `k` prefix.
- All enum variants must be in `PascalCase` with an `e` prefix.
- All macros must be in `SCREAMING_SNAKE_CASE` with an `MODULE_NAME_` prefix.
- All public field names must be in `camelCase`.
- All private field names must be in `PascalCase` with an `m` prefix.

## Physical structure

- All types must have a header file at
  `sources/<visibilityName>/<moduleName>/<packageName>/<TypeName>.hpp`.
- Non-type cross-platform header files must be placed at
  `sources/<visibilityName>/<moduleName>/<packageName>/<FileDescription>.hpp`.
- Non-type platform-specific header files must be placed at
  `sources/<visibilityName>/<moduleName>/<packageName>/<platformName>/<FileDescription>.hpp`.
- All types that need a cross-platform implementation file must place it at
  `sources/private/<moduleName>/<packageName>/<TypeName>.cpp`.
- All types that need a platform-specific implementation file must place it at
  `sources/private/<moduleName>/<packageName>/<platformName>/<TypeName>.cpp`.
- Non-type cross-platform implementation files must be placed at
  `sources/private/<moduleName>/<packageName>/<FileDescription>.cpp`.
- Non-type platform-specific implementation files must be placed at
  `sources/private/<moduleName>/<packageName>/<platformName>/<FileDescription>.cpp`.
- All executable modules must have the entrypoint at
  `sources/private/<moduleName>/main.cpp`.

## Format

- All functions must be in trailing return type format.
- All initialized variables must be declared with `auto`.
- All committed code must be formatted with `clang-format`.
- Some code sections can be manually formatted to improve readability. This is
  usually good for code that can be formatted into a table by making some
  statements stay on the same line.

## Build system

- Use latest possible version.
- All `clang` extensions can be used.
- All dependencies must be brought in via `FetchContent` of `CMake`.
- All dependencies must be accessed through custom targets which are named as
  `tomurcukCustom_<dependencyName>`.
- All non-modular targets must be named as `tomurcukCustom_<targetName>`.
- All packages must have a `CMake` target named
  `tomurcukPackage_<moduleName>__<packageName>`.
- All modules must have a `CMake` target named `tomurcukModule_<moduleName>`.
- All `CMake` variable names must be in `SCREAMING_SNAKE_CASE` with an
  `TOMURCUK_` prefix.

## Documentation

- All files must start with a comment that explains what it does, why it is
  needed, and how it should be used.
- Individual symbols should not be documented when created. Later, if it is seen
  that a symbol is too complex to understand under a couple of minutes, it can
  be briefly explained with a comment right before its declaration.
- Higher level design decisions could be explained in documents that are placed
  in `documents` directory.
