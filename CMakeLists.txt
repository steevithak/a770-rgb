cmake_minimum_required(VERSION 3.14)
project(a770-rgb VERSION 1.0.0 LANGUAGES C)

# look for standard package first
find_package(hidapi QUIET)

# If that fails try work-around for Ubuntu
if(NOT hidapi_FOUND)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(hidapi QUIET hidapi)
    if(NOT hidapi_FOUND)
        pkg_check_modules(hidapi REQUIRED hidapi-hidraw)
    endif()
endif()

add_executable(a770-rgb src/main.c src/hidutils.c src/ledzones.c)

target_link_libraries(a770-rgb PRIVATE hidapi::hidapi)

