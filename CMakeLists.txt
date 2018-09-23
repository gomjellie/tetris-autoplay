cmake_minimum_required(VERSION 3.7)
project(tetris_autoplay)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        tetris.c)

find_package(curses REQUIRED)

add_executable(tetris_autoplay ${SOURCE_FILES})

target_link_libraries(tetris_autoplay curses)
