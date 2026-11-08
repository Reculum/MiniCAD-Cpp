
cmake_minimum_required(VERSION 3.10)

project(MiniCAD LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(miniCAD main.cpp interactions.cpp)

target_include_directories(miniCAD PRIVATE
    /usr/local/include
)

target_link_directories(miniCAD PRIVATE
    /usr/local/lib
)

target_link_libraries(miniCAD PRIVATE
	raylib
    GL
    m
    pthread
    dl
    rt
    X11
)