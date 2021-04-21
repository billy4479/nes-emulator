cmake_minimum_required(VERSION 3.10)

project(nes-emulator LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

file( GLOB SOURCES src/*.cpp )

add_executable( nes-emulator ${SOURCES} )
target_link_libraries(nes-emulator PRIVATE SDL2::SDL2)