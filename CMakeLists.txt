cmake_minimum_required(VERSION 4.0)
project(Tomurcuk)

#-------------------------------------------------------------------------------
# Compiler configuration to be used

if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
    # cSpell: disable
    set(TOMURCUK_COMPILE_OPTIONS
        "-fno-exceptions"
        "-fno-rtti"
        "-std=gnu++2c"
        "-Weverything"
        "-Wno-c++98-compat"
        "-Wno-deprecated-declarations"
        "-Wno-old-style-cast"
        "-Wno-unsafe-buffer-usage-in-libc-call"
        "-Wno-writable-strings"
    )
    # cSpell: enable
else()
    message(FATAL_ERROR "Unsupported compiler frontend: ${CMAKE_CXX_COMPILER_FRONTEND_VARIANT}")
endif()

#-------------------------------------------------------------------------------
# The library

add_library(tomurcukLibrary)
target_compile_options(tomurcukLibrary PRIVATE ${TOMURCUK_COMPILE_OPTIONS})

target_include_directories(tomurcukLibrary
    PRIVATE "${CMAKE_SOURCE_DIR}/privateHeaders"
    PUBLIC "${CMAKE_SOURCE_DIR}/publicHeaders"
)

target_sources(tomurcukLibrary PRIVATE
    "${CMAKE_SOURCE_DIR}/sources/trk/Crashes.cpp"
    "${CMAKE_SOURCE_DIR}/sources/trk/Demo.cpp"
    "${CMAKE_SOURCE_DIR}/sources/trk/PrivateDemo.cpp"
    "${CMAKE_SOURCE_DIR}/sources/trk/StandardError.cpp"
)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_sources(tomurcukLibrary PRIVATE
        "${CMAKE_SOURCE_DIR}/sources/trk/windows/PlatformError.cpp"
    )
else()
    message(FATAL_ERROR "Unsupported system name: ${CMAKE_SYSTEM_NAME}")
endif()


#-------------------------------------------------------------------------------
# Tests for the library

add_executable(tomurcukTestExecutable EXCLUDE_FROM_ALL
    "${CMAKE_SOURCE_DIR}/sources/trkTest/main.cpp"
)

target_compile_options(tomurcukTestExecutable PRIVATE ${TOMURCUK_COMPILE_OPTIONS})
target_link_libraries(tomurcukTestExecutable PRIVATE tomurcukLibrary)
