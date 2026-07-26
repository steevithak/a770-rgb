cmake_minimum_required(VERSION 3.14)
project(a770-rgb VERSION 1.0.0 LANGUAGES C)

find_package(hidapi REQUIRED)

add_executable(a770-rgb src/main.c src/hidutils.c src/ledzones.c)

target_link_libraries(a770-rgb PRIVATE hidapi::hidapi)

