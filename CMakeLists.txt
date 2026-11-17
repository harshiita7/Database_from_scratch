cmake_minimum_required(VERSION 3.20)
project(ZeliaDB LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(zeliadb
    src/main.cpp
    src/storage/disk_manager.cpp
    src/storage/page.cpp
    src/storage/record.cpp
)

target_include_directories(zeliadb PRIVATE src)

if (MSVC)
    target_compile_options(zeliadb PRIVATE /W4 /permissive-)
else()
    target_compile_options(zeliadb PRIVATE -Wall -Wextra -Wpedantic)
endif()
