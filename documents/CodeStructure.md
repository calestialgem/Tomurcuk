# Code structure

## Logical structure

The codebase is divided into packages. These are named in `camelCase`. Packages
do not define a namespace: all symbols must be in the `tomurcuk` namespace.
Types in a package can be either private to the package or public to everybody.

Only types can be file-level symbols: all functions and global variables must be
a member of a type. Types must be named in `PascalCase`. Functions and local
variables must be named in `camelCase`. Private variables, private static
variables, public static variables, enum variants, and constants must be named
in `PascalCase` with `m`, `g`, `s`, `e`, or `k` prefix, respectively. Public
variables must be named in `camelCase`.

Macros must be in `SCREAMING_SNAKE_CASE` with `TOMURCUK_` prefix.

Latest possible version of CMake and clang can be assumed to be available with
all the compiler extensions.

All global CMake variable names must be in `SCREAMING_SNAKE_CASE` with
`TOMURCUK_` prefix.

Packages should be compiled into static libraries that are be named the same as
the package with a `tomurcukPackage_` prefix.

For convenience there should be a static library that contains all packages that
is named `tomurcuk`.

All dependencies must be accessed through custom targets which are named as
`tomurcukCustom_<dependencyName>`.

All non-code targets must be named as `tomurcukCustom_<targetName>`.

## Physical structure

All packages must have a subdirectory under `packages` top-level directory,
which has the same name as the package.

All types must be housed in files that have the same name as the type.

Files must go into a subdirectory named `public` or `private` under the
package's directory depending on the visibility of symbols defined inside them.
Platform-specific files must go to a subdirectory named the same as the platform
under the package's directory.

The files must be placed under a directory called `tomurcuk` so that all
includes of types, independent of their visibility, is done through
`<namespace/TypeName.hpp>`.

All dependencies must be brought in via `FetchContent` of CMake.
