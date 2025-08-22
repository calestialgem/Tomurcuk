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

set(TOMURCUK_PACKAGE_DIRECTORY "${CMAKE_SOURCE_DIR}/packages")

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
        "-Wno-missing-include-dirs"
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

function(tomurcukDefinePackage PACKAGE_NAME)
    set(OPTIONS "")
    set(ONE_VALUE_ARGS "")
    set(MULTI_VALUE_ARGS PACKAGE_DEPENDENCIES TRANSITIVE_PACKAGE_DEPENDENCIES CUSTOM_DEPENDENCIES TRANSITIVE_CUSTOM_DEPENDENCIES)
    cmake_parse_arguments(PARSE "${OPTIONS}" "${ONE_VALUE_ARGS}" "${MULTI_VALUE_ARGS}" ${ARGN}) # cSpell: disable-line

    foreach(i ${MULTI_VALUE_ARGS})
        if(NOT DEFINED PARSE_${i})
            set(PARSE_${i})
        endif()
    endforeach()

    if(PARSE_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unknown arguments in tomurcukDefinePackage: ${PARSE_UNPARSED_ARGUMENTS}")
    endif()

    set(PACKAGE_TARGET_NAME "tomurcukPackage_${PACKAGE_NAME}")
    list(TRANSFORM PARSE_PACKAGE_DEPENDENCIES PREPEND "tomurcukPackage_")
    list(TRANSFORM PARSE_TRANSITIVE_PACKAGE_DEPENDENCIES PREPEND "tomurcukPackage_")
    list(TRANSFORM PARSE_CUSTOM_DEPENDENCIES PREPEND "tomurcukCustom_")
    list(TRANSFORM PARSE_TRANSITIVE_CUSTOM_DEPENDENCIES PREPEND "tomurcukCustom_")
    set(PACKAGE_DIRECTORY "${TOMURCUK_PACKAGE_DIRECTORY}/${PACKAGE_NAME}")
    set(PACKAGE_PRIVATE_DIRECTORY "${PACKAGE_DIRECTORY}/private")
    set(PACKAGE_PUBLIC_DIRECTORY "${PACKAGE_DIRECTORY}/public")
    set(PACKAGE_PLATFORM_DIRECTORY "${PACKAGE_DIRECTORY}/${TOMURCUK_PLATFORM_NAME}")
    set(PACKAGE_ENTRYPOINT_FILE "${PACKAGE_DIRECTORY}/main.cpp")

    file(GLOB PACKAGE_HEADER_FILES CONFIGURE_DEPENDS
        "${PACKAGE_PRIVATE_DIRECTORY}/tomurcuk/*.hpp"
        "${PACKAGE_PUBLIC_DIRECTORY}/tomurcuk/*.hpp"
        "${PACKAGE_PLATFORM_DIRECTORY}/tomurcuk/*.hpp"
    )
    file(GLOB PACKAGE_IMPLEMENTATION_FILES CONFIGURE_DEPENDS
        "${PACKAGE_PRIVATE_DIRECTORY}/tomurcuk/*.cpp"
        "${PACKAGE_PLATFORM_DIRECTORY}/tomurcuk/*.cpp"
    )

    if (EXISTS ${PACKAGE_ENTRYPOINT_FILE} AND NOT IS_DIRECTORY ${PACKAGE_ENTRYPOINT_FILE})
        add_executable(${PACKAGE_TARGET_NAME} EXCLUDE_FROM_ALL ${PACKAGE_HEADER_FILES} ${PACKAGE_IMPLEMENTATION_FILES} ${PACKAGE_ENTRYPOINT_FILE})
        target_compile_options(${PACKAGE_TARGET_NAME} PRIVATE ${TOMURCUK_COMPILE_OPTIONS})
        target_link_libraries(${PACKAGE_TARGET_NAME} PRIVATE ${PARSE_PACKAGE_DEPENDENCIES} ${PARSE_CUSTOM_DEPENDENCIES} PUBLIC ${PARSE_TRANSITIVE_PACKAGE_DEPENDENCIES} ${PARSE_TRANSITIVE_CUSTOM_DEPENDENCIES})
        target_include_directories(${PACKAGE_TARGET_NAME} PRIVATE ${PACKAGE_PRIVATE_DIRECTORY} ${PACKAGE_PLATFORM_DIRECTORY} PUBLIC ${PACKAGE_PUBLIC_DIRECTORY})
    elseif(PACKAGE_IMPLEMENTATION_FILES)
        add_library(${PACKAGE_TARGET_NAME} EXCLUDE_FROM_ALL ${PACKAGE_HEADER_FILES} ${PACKAGE_IMPLEMENTATION_FILES})
        target_compile_options(${PACKAGE_TARGET_NAME} PRIVATE ${TOMURCUK_COMPILE_OPTIONS})
        target_link_libraries(${PACKAGE_TARGET_NAME} PRIVATE ${PARSE_PACKAGE_DEPENDENCIES} ${PARSE_CUSTOM_DEPENDENCIES} PUBLIC ${PARSE_TRANSITIVE_PACKAGE_DEPENDENCIES} ${PARSE_TRANSITIVE_CUSTOM_DEPENDENCIES})
        target_include_directories(${PACKAGE_TARGET_NAME} PRIVATE ${PACKAGE_PRIVATE_DIRECTORY} ${PACKAGE_PLATFORM_DIRECTORY} PUBLIC ${PACKAGE_PUBLIC_DIRECTORY})
    else()
        add_library(${PACKAGE_TARGET_NAME} INTERFACE EXCLUDE_FROM_ALL ${PACKAGE_HEADER_FILES})
        target_link_libraries(${PACKAGE_TARGET_NAME} INTERFACE ${PARSE_TRANSITIVE_PACKAGE_DEPENDENCIES} ${PARSE_TRANSITIVE_CUSTOM_DEPENDENCIES})
        target_include_directories(${PACKAGE_TARGET_NAME} INTERFACE ${PACKAGE_PUBLIC_DIRECTORY})
    endif()
endfunction()

#-------------------------------------------------------------------------------
# The library

tomurcukDefinePackage(status)

tomurcukDefinePackage(memory
    TRANSITIVE_PACKAGE_DEPENDENCIES
        status
)

tomurcukDefinePackage(data
    TRANSITIVE_PACKAGE_DEPENDENCIES
        memory
)

tomurcukDefinePackage(test
    PACKAGE_DEPENDENCIES
        memory
        data
    CUSTOM_DEPENDENCIES
        greatest
)
