cmake_minimum_required(VERSION 3.16)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(robot_quest C)

add_executable(robot_quest
    src/entity.c
    src/main.c
    src/title_screen.c
    res/sprites.c
)

target_include_directories(robot_quest
    PUBLIC
    ${CMAKE_SOURCE_DIR}
)
