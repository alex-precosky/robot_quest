cmake_minimum_required(VERSION 3.16)
set(CMAKE_VERBOSE_MAKEFILE ON)
project(robot_quest C)

add_executable(robot_quest
    src/main.c
    res/dungeon_map.c
    res/dungeon_tiles.c
)

target_include_directories(robot_quest
    PUBLIC
    ${CMAKE_SOURCE_DIR}
)
