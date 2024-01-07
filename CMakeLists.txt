cmake_minimum_required(VERSION 3.16)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(robot_quest C)

include(ExternalProject)

find_package(hUGEDriver)
find_package(CBT_FX)

add_fx_hammer_sfx_library(
 Sfx_Select
 FXSAV res/hammered.sav
 FXNUM 0
)

add_executable(robot_quest
    src/entity.c
    src/main.c
    src/title_screen.c
    res/sprites.c
    res/sample_song.c
)

target_include_directories(robot_quest
    PUBLIC
    ${HUGEDRIVER_INCLUDE_DIR}
    ${SFX_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(robot_quest
    hUGEDriver
    CBT_FX
    Sfx_Select
)
