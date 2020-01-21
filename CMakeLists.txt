cmake_minimum_required(VERSION 3.5)

project(untitled LANGUAGES CXX)
include_directories(${CMAKE_SOURCE_DIR})
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
file(GLOB_RECURSE SRCS "src/*.cpp")

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
ADD_EXECUTABLE(Game  ${SRCS})
target_link_libraries(Game ${CURSES_LIBRARY})
