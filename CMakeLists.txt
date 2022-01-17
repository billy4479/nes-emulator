cmake_minimum_required(VERSION 3.18)

project(nes-emulator LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

if(MSVC)
    find_package(SDL2 CONFIG REQUIRED)
    find_package(sdl2-image CONFIG REQUIRED)
    find_package(sdl2-ttf CONFIG REQUIRED)

    add_library(SDL2::Main ALIAS SDL2::SDL2main)
    add_library(SDL2::Image ALIAS SDL2::SDL2_image)
    add_library(SDL2::TTF ALIAS SDL2::SDL2_ttf)

else()
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2-cmake-modules)

    find_package(SDL2 REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_image REQUIRED)
endif()

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    message(STATUS "Found ccache")
    set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
endif()

message(STATUS "Getting glm ready...")
FetchContent_Declare(glm
    GIT_REPOSITORY "https://github.com/g-truc/glm"
    GIT_TAG "0.9.9.8"
)
FetchContent_MakeAvailable(glm)


file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable( ${PROJECT_NAME} ${SRC_FILES} )

target_link_libraries(${PROJECT_NAME}
    PUBLIC SDL2::Main SDL2::TTF SDL2::Image glm
)

target_compile_options(${PROJECT_NAME} PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>: -Wall -Wextra -pedantic -Werror>
)
