cmake_minimum_required(VERSION 3.10)

project(nes-emulator LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(CROSS_COMPILE_WINDOWS "Cross compile for windows" OFF)
# option(STATIC "Build a static executable" OFF)

find_package(SDL REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

file( GLOB SOURCES src/*.cpp )

add_executable( ${PROJECT_NAME} ${SOURCES} )

# if(STATIC)
#     target_link_libraries(${PROJECT_NAME} -static)
#     set_target_properties(${PROJECT_NAME} PROPERTIES LINK_SEARCH_START_STATIC 1)
#     set_target_properties(${PROJECT_NAME} PROPERTIES LINK_SEARCH_END_STATIC 1)
#     set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
#     #set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
# endif()

if(CROSS_COMPILE_WINDOWS)
    set(CMAKE_SYSTEM_NAME Windows)
    set(CMAKE_SYSTEM_VERSION 10.0)
    set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
    set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")

    target_link_libraries(${PROJECT_NAME} mingw32 SDL2main)
endif()

target_link_libraries(${PROJECT_NAME} SDL2 )
