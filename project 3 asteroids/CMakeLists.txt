cmake_minimum_required(VERSION 3.10)

# needed for Theo's editor
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set the project name
project(asteroids)

# get the sdl imports
find_package(SDL2 REQUIRED)

# configure output executable from given file
add_executable(asteroids main.cpp)

# genuinely idk, sort of loads sdl
target_link_libraries(SDL2_Example PRIVATE SDL2::SDL2)
