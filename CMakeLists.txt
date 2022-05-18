cmake_minimum_required(VERSION 3.23)
set(
  CMAKE_CXX_STANDARD 17
  CMAKE_CXX_STANDARD_REQUIRED ON
)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

project(2048)
add_executable(2048 src/main.cpp)
target_link_libraries(2048 ${CURSES_LIBRARIES})
