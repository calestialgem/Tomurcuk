cmake_minimum_required(VERSION 4.0)
project(Tomurcuk)

#-------------------------------------------------------------------------------
# CMake configuration

include(FetchContent)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(TOMURCUK_PLATFORM_NAME "windows")
else()
    message(FATAL_ERROR "Unsupported system name: ${CMAKE_SYSTEM_NAME}")
endif()

set(TOMURCUK_SOURCE_DIRECTORY "${CMAKE_SOURCE_DIR}/sources")
set(TOMURCUK_PRIVATE_SOURCE_DIRECTORY "${TOMURCUK_SOURCE_DIRECTORY}/private")
set(TOMURCUK_PUBLIC_SOURCE_DIRECTORY "${TOMURCUK_SOURCE_DIRECTORY}/public")

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

add_library(tomurcukCustom_greatest INTERFACE)
target_sources(tomurcukCustom_greatest INTERFACE "${greatest_SOURCE_DIR}/greatest.h")
target_include_directories(tomurcukCustom_greatest INTERFACE "${greatest_SOURCE_DIR}")
target_compile_definitions(tomurcukCustom_greatest INTERFACE "GREATEST_USE_ABBREVS=0")

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
# Helper function for defining targets

function(tomurcukDefineModule MODULE_NAME)
    set(OPTIONS "")
    set(ONE_VALUE_ARGS "")
    set(MULTI_VALUE_ARGS MODULE_DEPENDENCIES CUSTOM_DEPENDENCIES)
    cmake_parse_arguments(PARSE "${OPTIONS}" "${ONE_VALUE_ARGS}" "${MULTI_VALUE_ARGS}" ${ARGN}) # cSpell: disable-line

    foreach(i ${MULTI_VALUE_ARGS})
        if(NOT DEFINED PARSE_${i})
            set(PARSE_${i})
        endif()
    endforeach()

    if(PARSE_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unknown arguments in tomurcukDefineModule: ${PARSE_UNPARSED_ARGUMENTS}")
    endif()

    set(MODULE_TARGET_NAME "tomurcukModule_${MODULE_NAME}")
    list(TRANSFORM PARSE_MODULE_DEPENDENCIES PREPEND "tomurcukModule_")
    list(TRANSFORM PARSE_CUSTOM_DEPENDENCIES PREPEND "tomurcukCustom_")
    set(MODULE_PRIVATE_SOURCE_DIRECTORY "${TOMURCUK_PRIVATE_SOURCE_DIRECTORY}/${MODULE_NAME}")
    set(MODULE_PUBLIC_SOURCE_DIRECTORY "${TOMURCUK_PUBLIC_SOURCE_DIRECTORY}/${MODULE_NAME}")
    set(MODULE_ENTRYPOINT "${MODULE_PRIVATE_SOURCE_DIRECTORY}/main.cpp")

    set(MODULE_PACKAGE_TARGET_NAMES "")

    file(GLOB PRIVATE_ORIGINATED_PACKAGE_NAMES RELATIVE ${MODULE_PRIVATE_SOURCE_DIRECTORY} CONFIGURE_DEPENDS "${MODULE_PRIVATE_SOURCE_DIRECTORY}/*")
    foreach(PACKAGE_NAME ${PRIVATE_ORIGINATED_PACKAGE_NAMES})
        set(PACKAGE_PRIVATE_SOURCE_DIRECTORY "${MODULE_PRIVATE_SOURCE_DIRECTORY}/${PACKAGE_NAME}")
        if(NOT IS_DIRECTORY ${PACKAGE_PRIVATE_SOURCE_DIRECTORY})
            continue()
        endif()
        set(PACKAGE_TARGET_NAME "tomurcukPackage_${MODULE_NAME}__${PACKAGE_NAME}")
        set(PACKAGE_PUBLIC_SOURCE_DIRECTORY "${MODULE_PUBLIC_SOURCE_DIRECTORY}/${PACKAGE_NAME}")
        set(PACKAGE_PRIVATE_PLATFORM_SOURCE_DIRECTORY "${PACKAGE_PRIVATE_SOURCE_DIRECTORY}/${TOMURCUK_PLATFORM_NAME}")
        set(PACKAGE_PUBLIC_PLATFORM_SOURCE_DIRECTORY "${PACKAGE_PUBLIC_SOURCE_DIRECTORY}/${TOMURCUK_PLATFORM_NAME}")
        file(GLOB PACKAGE_HEADER_SOURCES CONFIGURE_DEPENDS
            "${PACKAGE_PRIVATE_SOURCE_DIRECTORY}/*.hpp"
            "${PACKAGE_PUBLIC_SOURCE_DIRECTORY}/*.hpp"
            "${PACKAGE_PRIVATE_PLATFORM_SOURCE_DIRECTORY}/*.hpp"
            "${PACKAGE_PUBLIC_PLATFORM_SOURCE_DIRECTORY}/*.hpp"
        )
        file(GLOB PACKAGE_IMPLEMENTATION_SOURCES CONFIGURE_DEPENDS
            "${PACKAGE_PRIVATE_SOURCE_DIRECTORY}/*.cpp"
            "${PACKAGE_PRIVATE_PLATFORM_SOURCE_DIRECTORY}/*.cpp"
        )

        if(PACKAGE_IMPLEMENTATION_SOURCES)
            add_library(${PACKAGE_TARGET_NAME} EXCLUDE_FROM_ALL ${PACKAGE_HEADER_SOURCES} ${PACKAGE_IMPLEMENTATION_SOURCES})
            target_compile_options(${PACKAGE_TARGET_NAME} PRIVATE ${TOMURCUK_COMPILE_OPTIONS})
            target_include_directories(${PACKAGE_TARGET_NAME} PUBLIC ${TOMURCUK_PUBLIC_SOURCE_DIRECTORY})
            target_link_libraries(${PACKAGE_TARGET_NAME} PRIVATE ${PARSE_MODULE_DEPENDENCIES} ${PARSE_CUSTOM_DEPENDENCIES})
        else()
            add_library(${PACKAGE_TARGET_NAME} INTERFACE EXCLUDE_FROM_ALL ${PACKAGE_HEADER_SOURCES})
            target_include_directories(${PACKAGE_TARGET_NAME} INTERFACE ${TOMURCUK_PUBLIC_SOURCE_DIRECTORY})
            target_link_libraries(${PACKAGE_TARGET_NAME} INTERFACE ${PARSE_MODULE_DEPENDENCIES} ${PARSE_CUSTOM_DEPENDENCIES})
        endif()

        list(APPEND MODULE_PACKAGE_TARGET_NAMES ${PACKAGE_TARGET_NAME})
    endforeach()

    file(GLOB PUBLIC_ORIGINATED_PACKAGE_NAMES RELATIVE ${MODULE_PUBLIC_SOURCE_DIRECTORY} CONFIGURE_DEPENDS "${MODULE_PUBLIC_SOURCE_DIRECTORY}/*")
    foreach(PACKAGE_NAME ${PUBLIC_ORIGINATED_PACKAGE_NAMES})
        set(PACKAGE_PUBLIC_SOURCE_DIRECTORY "${MODULE_PUBLIC_SOURCE_DIRECTORY}/${PACKAGE_NAME}")
        if(NOT IS_DIRECTORY ${PACKAGE_PUBLIC_SOURCE_DIRECTORY})
            continue()
        endif()
        set(PACKAGE_PRIVATE_SOURCE_DIRECTORY "${MODULE_PRIVATE_SOURCE_DIRECTORY}/${PACKAGE_NAME}")
        if(IS_DIRECTORY ${PACKAGE_PRIVATE_SOURCE_DIRECTORY})
            continue()
        endif()
        set(PACKAGE_TARGET_NAME "tomurcukPackage_${MODULE_NAME}__${PACKAGE_NAME}")
        set(PACKAGE_PUBLIC_PLATFORM_SOURCE_DIRECTORY "${PACKAGE_PUBLIC_SOURCE_DIRECTORY}/${TOMURCUK_PLATFORM_NAME}")
        file(GLOB PACKAGE_HEADER_SOURCES CONFIGURE_DEPENDS
            "${PACKAGE_PUBLIC_SOURCE_DIRECTORY}/*.hpp"
            "${PACKAGE_PUBLIC_PLATFORM_SOURCE_DIRECTORY}/*.hpp"
        )

        add_library(${PACKAGE_TARGET_NAME} INTERFACE EXCLUDE_FROM_ALL ${PACKAGE_HEADER_SOURCES})
        target_include_directories(${PACKAGE_TARGET_NAME} INTERFACE ${TOMURCUK_PUBLIC_SOURCE_DIRECTORY})
        target_link_libraries(${PACKAGE_TARGET_NAME} INTERFACE ${PARSE_MODULE_DEPENDENCIES} ${PARSE_CUSTOM_DEPENDENCIES})

        list(APPEND MODULE_PACKAGE_TARGET_NAMES ${PACKAGE_TARGET_NAME})
    endforeach()

    if(EXISTS ${MODULE_ENTRYPOINT} AND NOT IS_DIRECTORY ${MODULE_ENTRYPOINT})
        add_executable(${MODULE_TARGET_NAME} EXCLUDE_FROM_ALL ${MODULE_ENTRYPOINT})
        target_compile_options(${MODULE_TARGET_NAME} PRIVATE ${TOMURCUK_COMPILE_OPTIONS})
        target_link_libraries(${MODULE_TARGET_NAME} PRIVATE ${MODULE_PACKAGE_TARGET_NAMES} ${PARSE_MODULE_DEPENDENCIES} ${PARSE_CUSTOM_DEPENDENCIES})
    else()
        add_library(${MODULE_TARGET_NAME} INTERFACE EXCLUDE_FROM_ALL)
        target_link_libraries(${MODULE_TARGET_NAME} INTERFACE ${MODULE_PACKAGE_TARGET_NAMES} ${PARSE_MODULE_DEPENDENCIES} ${PARSE_CUSTOM_DEPENDENCIES})
    endif()
endfunction()

#-------------------------------------------------------------------------------
# The library

tomurcukDefineModule(tomurcuk)

#-------------------------------------------------------------------------------
# Tests for the library

tomurcukDefineModule(tomurcukTest
    MODULE_DEPENDENCIES
        tomurcuk
    CUSTOM_DEPENDENCIES
        greatest
)
