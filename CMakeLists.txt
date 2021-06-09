cmake_minimum_required(VERSION 3.16)
project(chip8)

find_package(SDL2 REQUIRED)

add_executable(chip8 
    source/chip8.cpp
    source/SDLIMP.cpp
    source/main.cpp)

target_compile_options(chip8 PRIVATE -Wall)

target_link_libraries(chip8 PRIVATE SDL2::SDL2)