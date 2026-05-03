cmake_minimum_required(VERSION 3.10)

# needed for Theo's editor
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set the project name
project(asteroids)

# get the sdl imports
find_package(SDL2 REQUIRED)


add_executable(asteroids main.cpp engine.cpp spritesheet.cpp)

# only compiles when the all_tests group is picked or one is specifically picked
add_executable(physics_test EXCLUDE_FROM_ALL physics_test.cpp engine.cpp spritesheet.cpp)
add_executable(rendering_test EXCLUDE_FROM_ALL rendering_test.cpp engine.cpp spritesheet.cpp)
add_executable(controls_test EXCLUDE_FROM_ALL controls_test.cpp engine.cpp spritesheet.cpp)

# Create a custom target to group them
add_custom_target(all_tests)
add_dependencies(all_tests physics_test rendering_test controls_test)

# genuinely idk, sort of loads sdl
target_link_libraries(asteroids PRIVATE SDL2::SDL2 SDL2_image)
target_link_libraries(rendering_test PRIVATE SDL2::SDL2 SDL2_image)
target_link_libraries(physics_test PRIVATE SDL2::SDL2 SDL2_image)
target_link_libraries(controls_test PRIVATE SDL2::SDL2 SDL2_image)


