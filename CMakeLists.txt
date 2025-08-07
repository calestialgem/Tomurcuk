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
        "-Wno-c++98-compat-pedantic"
        "-Wno-deprecated-declarations"
        "-Wno-old-style-cast"
        "-Wno-unsafe-buffer-usage"
        "-Wno-unsafe-buffer-usage-in-libc-call"
        "-Wno-writable-strings"
    )
    # cSpell: enable
else()
    message(FATAL_ERROR "Unsupported compiler frontend: ${CMAKE_CXX_COMPILER_FRONTEND_VARIANT}")
endif()

#-------------------------------------------------------------------------------
# Helper functions for defining targets

function(tomurcukDefinePackage MODULE_NAME PACKAGE_NAME)
    set(OPTIONS "")
    set(ONE_VALUE_ARGS "")
    set(MULTI_VALUE_ARGS SOURCES WINDOWS_SOURCES)
    cmake_parse_arguments(PARSE "${OPTIONS}" "${ONE_VALUE_ARGS}" "${MULTI_VALUE_ARGS}" ${ARGN}) # cSpell: disable-line

    foreach(i ${MULTI_VALUE_ARGS})
        if(NOT DEFINED PARSE_${i})
            set(PARSE_${i})
        endif()
    endforeach()

    if(PARSE_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unknown arguments in tomurcukDefinePackage: ${PARSE_UNPARSED_ARGUMENTS}")
    endif()

    set(TARGET_NAME "package${MODULE_NAME}${PACKAGE_NAME}")
    list(TRANSFORM PARSE_SOURCES PREPEND "${CMAKE_SOURCE_DIR}/private/${MODULE_NAME}/${PACKAGE_NAME}/")
    list(TRANSFORM PARSE_WINDOWS_SOURCES PREPEND "${CMAKE_SOURCE_DIR}/private/${MODULE_NAME}/${PACKAGE_NAME}/windows/")

    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        list(APPEND PARSE_SOURCES ${PARSE_WINDOWS_SOURCES})
    else()
        message(FATAL_ERROR "Unsupported system name: ${CMAKE_SYSTEM_NAME}")
    endif()

    if(PARSE_SOURCES)
        add_library(${TARGET_NAME} EXCLUDE_FROM_ALL ${PARSE_SOURCES})
        target_compile_options(${TARGET_NAME} PRIVATE ${TOMURCUK_COMPILE_OPTIONS})
        target_include_directories(${TARGET_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/public")
    else()
        add_library(${TARGET_NAME} INTERFACE EXCLUDE_FROM_ALL)
        target_include_directories(${TARGET_NAME} INTERFACE "${CMAKE_SOURCE_DIR}/public")
    endif()
endfunction()

function(tomurcukDefineModule MODULE_NAME)
    set(OPTIONS EXECUTABLE)
    set(ONE_VALUE_ARGS "")
    set(MULTI_VALUE_ARGS PACKAGES DEPENDENCIES)
    cmake_parse_arguments(PARSE "${OPTIONS}" "${ONE_VALUE_ARGS}" "${MULTI_VALUE_ARGS}" ${ARGN}) # cSpell: disable-line

    foreach(i ${MULTI_VALUE_ARGS})
        if(NOT DEFINED PARSE_${i})
            set(PARSE_${i})
        endif()
    endforeach()

    if(PARSE_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unknown arguments in tomurcukDefineModule: ${PARSE_UNPARSED_ARGUMENTS}")
    endif()

    set(TARGET_NAME "module${MODULE_NAME}")
    list(TRANSFORM PARSE_PACKAGES PREPEND "package${MODULE_NAME}")
    list(TRANSFORM PARSE_DEPENDENCIES PREPEND "module")

    if(PARSE_EXECUTABLE)
        add_executable(${TARGET_NAME} EXCLUDE_FROM_ALL "${CMAKE_SOURCE_DIR}/private/${MODULE_NAME}/main.cpp")
        target_compile_options(${TARGET_NAME} PRIVATE ${TOMURCUK_COMPILE_OPTIONS})
        target_link_libraries(${TARGET_NAME} PRIVATE ${PARSE_PACKAGES} ${PARSE_DEPENDENCIES})
    else()
        add_library(${TARGET_NAME} INTERFACE EXCLUDE_FROM_ALL)
        target_link_libraries(${TARGET_NAME} INTERFACE ${PARSE_PACKAGES} ${PARSE_DEPENDENCIES})
    endif()
endfunction()

#-------------------------------------------------------------------------------
# The library

tomurcukDefinePackage(Tomurcuk Base
    SOURCES
        "Bytes.cpp"
        "MemoryAllocator.cpp"
        "VirtualBlock.cpp"
    WINDOWS_SOURCES
        "Bytes.cpp"
        "VirtualBlock.cpp"
)

tomurcukDefinePackage(Tomurcuk Demo
    SOURCES
        "Demo.cpp"
        "PrivateDemo.cpp"
)

tomurcukDefinePackage(Tomurcuk Status
    SOURCES
        "Crashes.cpp"
        "StandardError.cpp"
    WINDOWS_SOURCES
        "PlatformError.cpp"
)

tomurcukDefineModule(Tomurcuk PACKAGES Base Demo Status)

#-------------------------------------------------------------------------------
# Tests for the library

tomurcukDefineModule(TomurcukTest EXECUTABLE DEPENDENCIES Tomurcuk)
