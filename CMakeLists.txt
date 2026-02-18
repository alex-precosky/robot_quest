cmake_minimum_required(VERSION 3.16)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(robot_quest C)

include(ExternalProject)

find_package(hUGEDriver)
find_package(CBT_FX)

# Sfx when pressing D-Pad L/R to choose a bot in the title screen
add_fx_hammer_sfx_library(
 Sfx_Select
 FXSAV res/hammered.sav
 FXNUM 0
)

# Sfx when pressing START to choose a bot in the title selection screen
add_fx_hammer_sfx_library(
 Sfx_BotChosen
 FXSAV res/hammered.sav
 FXNUM 1
)

add_executable(robot_quest
    src/entity.c
    src/level.c
    src/main.c
    src/sprite_idx.c
    src/title_screen.c
    res/level_map.c
    res/level_map_tiles.c
    res/sprites.c
    res/sample_song.c
)

target_include_directories(robot_quest
    PUBLIC
    ${HUGEDRIVER_INCLUDE_DIR}
    ${SFX_INCLUDE_DIR}
    $ENV{GBDK_HOME}/include
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(robot_quest
    hUGEDriver
    CBT_FX
    Sfx_Select
    Sfx_BotChosen
)
