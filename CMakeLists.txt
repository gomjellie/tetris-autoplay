cmake_minimum_required(VERSION 3.7)
project(tetris_autoplay)
include(ExternalProject)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "{CMAKE_CXX_FLAGS} -P")

set(SOURCE_FILES
        tetris.c)

set(curses_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/libncurses5")
set(curses_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/libncurses5")

IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  SET(CMAKE_INSTALL_PREFIX <path> CACHE PATH <comment> FORCE)
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

# find_package(curses REQUIRED)

ExternalProject_add(project_curses
	URL http://ftp.gnu.org/pub/gnu/ncurses/ncurses-6.1.tar.gz
	PREFIX ${curses_PREFIX}
	CONFIGURE_COMMAND ${curses_PREFIX}/src/project_curses/configure --prefix=${curses_INSTALL_DIR}
	PREFIX=${curses_PREFIX}
	BUILD_COMMAND make
	INSTALL_COMMAND make install
	#INSTALL_DIR ${curses_INSTALL_DIR}
)

ExternalProject_Get_Property(project_curses install_dir)

add_library(curses STATIC IMPORTED)

set_property(TARGET curses PROPERTY IMPORTED_LOCATION ${install_dir}/lib/libncurses5)

add_dependencies(curses project_curses)

add_executable(tetris_autoplay ${SOURCE_FILES})

include_directories(${install_dir}/include/include/libncurses5)

target_link_libraries(tetris_autoplay curses)

