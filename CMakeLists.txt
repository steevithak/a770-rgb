cmake_minimum_required(VERSION 3.14)
project(a770-rgb VERSION 1.0.0 LANGUAGES C)

# look for standard package first
find_package(hidapi QUIET)

# Work-around because find_package doesn't work on Ubuntu
if(NOT hidapi_FOUND)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(hidapi QUIET hidapi)
    if(NOT hidapi_FOUND)
        pkg_check_modules(hidapi REQUIRED hidapi-hidraw)
    endif()
endif()

# Work-around because Ubuntu can't find libs either
if(NOT TARGET hidapi::hidapi)
    add_library(hidapi::hidapi INTERFACE IMPORTED)
    set_target_properties(hidapi::hidapi PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${hidapi_INCLUDE_DIRS}"
        INTERFACE_LINK_LIBRARIES "${hidapi_LIBRARIES}"
        INTERFACE_COMPILE_OPTIONS "${hidapi_CFLAGS_OTHER}"
    )
endif()

add_executable(a770-rgb src/main.c src/hidutils.c src/ledzones.c)

target_link_libraries(a770-rgb PRIVATE hidapi::hidapi)

