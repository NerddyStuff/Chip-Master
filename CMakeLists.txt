cmake_minimum_required(VERSION 3.16)
project(CHIP8)

find_package(SDL2 REQUIRED)

add_executable(CHIP8 
    source/chip8.cpp
    source/SDLIMP.cpp
    source/main.cpp)

target_compile_options(CHIP8 PRIVATE -Wall)

target_link_libraries(CHIP8 PRIVATE SDL2::SDL2)