# CBT-FX is a sound effects driver
# Exposes library CBT_FX and function add_fx_hammer_sfx_library()

include(FetchContent)

FetchContent_Declare(CBT_FX_repo
  GIT_REPOSITORY https://github.com/coffeevalenbat/CBT-FX.git
  GIT_TAG a2d20b8e74226b30d14a3928245f96e7bd986cd1

  # CBT_FX calls into hUGEDriver's assembly library to mute and unmute sound
  # channels, but using the pre-SDCC 4.1 calling convention, whereas
  # hUGEDriver's assembly is using the new calling convention
  PATCH_COMMAND git apply ${CMAKE_CURRENT_LIST_DIR}/0001-sdcc_use_new_calling_convention.patch
  UPDATE_COMMAND "" # Don't update, and therefore, reapply the patch on each build
)
FetchContent_MakeAvailable(CBT_FX_repo)

FetchContent_GetProperties(CBT_FX_repo SOURCE_DIR CBT_FX_SOURCE_DIR)

add_library(CBT_FX STATIC
  ${CBT_FX_SOURCE_DIR}/include/cbtfx.c
)

target_include_directories(CBT_FX
  PUBLIC
  ${CBT_FX_SOURCE_DIR}/include
)

# Build a library from a sound effect stored in an FX-Hammer save file
#
# add_fx_hammer_lib(LIBRARY_NAME FXSAV FXNUM)
# Parameters:
# LIBRARY_NAME The name of the library target to create
# FXSAV: The path to the FX-HAMMER save file
# FXNUM: The sound effect number to export
#
# To consume the library in a target:
#
# target_link_libraries(<my target> <library name)
# target_include_directories(<my target>
#   ${SFX_INCLUDE_DIR}
# )
function(add_fx_hammer_sfx_library LIBRARY_NAME)
  cmake_parse_arguments(FX_HAMMER "" "FXSAV;FXNUM" "" ${ARGN})

  find_package(Python COMPONENTS Interpreter)

  # FX-Hammer provides a python script to export its .sav files to .c/.h files
  set(HAMMER2CBT_PY ${CBT_FX_SOURCE_DIR}/hammer2cbt.py)

  # Where to save hammer2cbt.py's output files
  set(HAMMERFX_SFX_DIR ${CMAKE_BINARY_DIR}/fxhammer_sfx)

  # hammer2cbt's output files are zero-padded
  if(FX_HAMMER_FXNUM LESS 10)
    set(SFX_OUTPUT_BASE_NAME SFX_0${FX_HAMMER_FXNUM})
  else()
    set(SFX_OUTPUT_BASE_NAME SFX_${FX_HAMMER_FXNUM})
  endif()

  set(HAMMERFX_OUTPUT_FILES ${HAMMERFX_SFX_DIR}/${SFX_OUTPUT_BASE_NAME}.c ${HAMMERFX_SFX_DIR}/${SFX_OUTPUT_BASE_NAME}.h)

  add_custom_command(OUTPUT ${HAMMERFX_OUTPUT_FILES}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${HAMMERFX_SFX_DIR}
    COMMAND ${Python_EXECUTABLE} ${HAMMER2CBT_PY} ${CMAKE_SOURCE_DIR}/${FX_HAMMER_FXSAV} ${FX_HAMMER_FXNUM} ${HAMMERFX_SFX_DIR}/
  )

  add_library(${LIBRARY_NAME}
    ${HAMMERFX_OUTPUT_FILES}
  )
  set(SFX_INCLUDE_DIR ${HAMMERFX_SFX_DIR} PARENT_SCOPE)
endfunction()
