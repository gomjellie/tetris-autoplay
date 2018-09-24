cmake_minimum_required(VERSION 3.7)
project(tetris_autoplay)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        tetris.c)

# find_package(curses REQUIRED)
include(ExternalProject)
ExternalProject_Add(project_curses
	URL http://archive.ubuntu.com/ubuntu/pool/main/n/ncurses/ncurses_5.9+20140118.orig.tar.gz
	PREFIX ${CMAKE_CURRENT_BINARY_DIR}/libncurses5
	CONFIGURE_COMMAND "${CMAKE_CURRENT_BINARY_DIR}/libncurses5/src/project_curses/configure"
	BUILD_COMMAND make
	INSTALL_COMMAND make install
	PREFIX=${CMAKE_CURRENT_BINARY_DIR}/libncurses5
)

ExternalProject_Get_Property(project_curses install_dir)

add_library(curses STATIC IMPORTED)

set_property(TARGET curses PROPERTY IMPORTED_LOCATION ${install_dir}/lib/libncurses5)

add_dependencies(curses project_curses)

add_executable(tetris_autoplay ${SOURCE_FILES})

include_directories(${install_dir}/include/include/libncurses5)

target_link_libraries(tetris_autoplay curses)

