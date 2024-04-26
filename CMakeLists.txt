cmake_minimum_required(VERSION 3.27)
project(Tetris)

set(CMAKE_CXX_STANDARD 17)

add_executable(Tetris tetris.cpp miniwin.cpp miniwin.h
)

target_link_libraries(Tetris winmm)

