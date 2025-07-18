cmake_minimum_required(VERSION 4.0)
project(Tomurcuk)

#-------------------------------------------------------------------------------
# Compiler configuration to be used

if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU") # cSpell: disable-line
    # cSpell: disable
    set(TRK_COMPILE_OPTIONS
        "-std=gnu++2c"
        "-Weverything"
        "-Wno-c++98-compat"
        "-Wno-unsafe-buffer-usage-in-libc-call"
    )
    # cSpell: enable
else()
    message(FATAL_ERROR "Unsupported compiler frontend: ${CMAKE_CXX_COMPILER_FRONTEND_VARIANT}")
endif()

#-------------------------------------------------------------------------------
# The library

add_library(trkLibrary)
target_compile_options(trkLibrary PRIVATE ${TRK_COMPILE_OPTIONS})

target_include_directories(trkLibrary
    PRIVATE "${CMAKE_SOURCE_DIR}/privateHeaders"
    PUBLIC "${CMAKE_SOURCE_DIR}/publicHeaders"
)

target_sources(trkLibrary PRIVATE
    "${CMAKE_SOURCE_DIR}/sources/trk/Demo.cpp"
    "${CMAKE_SOURCE_DIR}/sources/trk/PrivateDemo.cpp"
)

#-------------------------------------------------------------------------------
# Tests for the library

add_executable(trkTestExecutable EXCLUDE_FROM_ALL
    "${CMAKE_SOURCE_DIR}/sources/trkTest/main.cpp"
)

target_compile_options(trkTestExecutable PRIVATE ${TRK_COMPILE_OPTIONS})
target_link_libraries(trkTestExecutable PRIVATE trkLibrary)
