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

## Format

- All functions must be in trailing return type format.
- All initialized variables must be declared with `auto`.
- All committed code must be formatted with `clang-format`.
- Some code sections can be manually formatted to improve readability. This is
  usually good for code that can be formatted into a table by making some
  statements stay on the same line.
