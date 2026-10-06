cmake_minimum_required(VERSION 3.15)
project(TetrisCLI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(tetris
    src/main.cpp
    src/Tetromino.cpp
    src/Board.cpp
    src/InputHandler.cpp
    src/Renderer.cpp
    src/Game.cpp
)

target_include_directories(tetris PRIVATE src)

if(MSVC)
    target_compile_options(tetris PRIVATE /W4 /permissive-)
else()
    target_compile_options(tetris PRIVATE -Wall -Wextra -pedantic)
endif()
