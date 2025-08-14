cmake_minimum_required(VERSION 4.0)
project(Tomurcuk)

#-------------------------------------------------------------------------------
# CMake configuration

include(FetchContent)

#-------------------------------------------------------------------------------
# Dependency: greatest

FetchContent_Declare(
    greatest
    GIT_REPOSITORY https://github.com/silentbicycle/greatest.git
    GIT_TAG 11a6af1919049df502405913da64fb385c219361
    GIT_SHALLOW on
    GIT_PROGRESS on
)
FetchContent_MakeAvailable(greatest)

add_library(targetGreatest INTERFACE)
target_sources(targetGreatest INTERFACE "${greatest_SOURCE_DIR}/greatest.h")
target_include_directories(targetGreatest INTERFACE "${greatest_SOURCE_DIR}")
target_compile_definitions(targetGreatest INTERFACE "GREATEST_USE_ABBREVS=0")

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
        "-Wno-reserved-macro-identifier"
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
    set(MULTI_VALUE_ARGS SOURCES WINDOWS_SOURCES MODULE_DEPENDENCIES TARGET_DEPENDENCIES)
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
    list(TRANSFORM PARSE_MODULE_DEPENDENCIES PREPEND "module")
    list(TRANSFORM PARSE_TARGET_DEPENDENCIES PREPEND "target")

    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        list(APPEND PARSE_SOURCES ${PARSE_WINDOWS_SOURCES})
    else()
        message(FATAL_ERROR "Unsupported system name: ${CMAKE_SYSTEM_NAME}")
    endif()

    if(PARSE_SOURCES)
        add_library(${TARGET_NAME} EXCLUDE_FROM_ALL ${PARSE_SOURCES})
        target_compile_options(${TARGET_NAME} PRIVATE ${TOMURCUK_COMPILE_OPTIONS})
        target_include_directories(${TARGET_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/public")
        target_link_libraries(${TARGET_NAME} PRIVATE ${PARSE_MODULE_DEPENDENCIES} ${PARSE_TARGET_DEPENDENCIES})
    else()
        add_library(${TARGET_NAME} INTERFACE EXCLUDE_FROM_ALL)
        target_include_directories(${TARGET_NAME} INTERFACE "${CMAKE_SOURCE_DIR}/public")
        target_link_libraries(${TARGET_NAME} INTERFACE ${PARSE_MODULE_DEPENDENCIES} ${PARSE_TARGET_DEPENDENCIES})
    endif()
endfunction()

function(tomurcukDefineModule MODULE_NAME)
    set(OPTIONS EXECUTABLE)
    set(ONE_VALUE_ARGS "")
    set(MULTI_VALUE_ARGS PACKAGES MODULE_DEPENDENCIES TARGET_DEPENDENCIES)
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
    list(TRANSFORM PARSE_MODULE_DEPENDENCIES PREPEND "module")
    list(TRANSFORM PARSE_TARGET_DEPENDENCIES PREPEND "target")

    if(PARSE_EXECUTABLE)
        add_executable(${TARGET_NAME} EXCLUDE_FROM_ALL "${CMAKE_SOURCE_DIR}/private/${MODULE_NAME}/main.cpp")
        target_compile_options(${TARGET_NAME} PRIVATE ${TOMURCUK_COMPILE_OPTIONS})
        target_link_libraries(${TARGET_NAME} PRIVATE ${PARSE_PACKAGES} ${PARSE_MODULE_DEPENDENCIES} ${PARSE_TARGET_DEPENDENCIES})
    else()
        add_library(${TARGET_NAME} INTERFACE EXCLUDE_FROM_ALL)
        target_link_libraries(${TARGET_NAME} INTERFACE ${PARSE_PACKAGES} ${PARSE_MODULE_DEPENDENCIES} ${PARSE_TARGET_DEPENDENCIES})
    endif()
endfunction()

#-------------------------------------------------------------------------------
# The library

tomurcukDefinePackage(Tomurcuk Base
    SOURCES
        "Bytes.cpp"
        "Hasher.cpp"
        "LinearMemoryAllocator.cpp"
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

tomurcukDefineModule(Tomurcuk
    PACKAGES
        Base
        Demo
        Status
)

#-------------------------------------------------------------------------------
# Tests for the library

tomurcukDefinePackage(TomurcukTest Base
    SOURCES
        "LinearMemoryAllocator.cpp"
    MODULE_DEPENDENCIES
        Tomurcuk
    TARGET_DEPENDENCIES
        Greatest
)

tomurcukDefineModule(TomurcukTest EXECUTABLE
    PACKAGES
        Base
    TARGET_DEPENDENCIES
        Greatest
)
