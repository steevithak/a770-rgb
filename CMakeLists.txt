cmake_minimum_required(VERSION 3.14)
project(a770-rgb VERSION 1.0.0 LANGUAGES C)

# static build for max portability
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

include(FetchContent)

# Fetch libusb/hidapi library
FetchContent_Declare(
  hidapi
  GIT_REPOSITORY https://github.com/libusb/hidapi
  GIT_TAG        master
)
FetchContent_MakeAvailable(hidapi)

add_executable(a770-rgb src/main.c)

target_link_libraries(a770-rgb PRIVATE hidapi::hidapi)

target_include_directories(a770-rgb PRIVATE
    ${hidapi_SOURCE_DIR}/hidapi
)

