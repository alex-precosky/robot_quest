# hUGEDriver is a music driver
# This package exposes target 'hUGEDriver' and variable ${HUGEDRIVER_INCLUDE_DIR)

ExternalProject_Add(hUGEDriver_repo
  GIT_REPOSITORY https://github.com/SuperDisk/hUGEDriver.git
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
)

ExternalProject_Get_property(hUGEDriver_repo SOURCE_DIR)
set(HUGEDRIVER_SOURCE_DIR ${SOURCE_DIR})

ExternalProject_Get_property(hUGEDriver_repo BINARY_DIR)
set(HUGEDRIVER_BINARY_DIR ${BINARY_DIR})

find_package(Python COMPONENTS Interpreter)

find_program(RGBASM rgbasm)
message("Found rgbasm at ${RGBASM}")

find_program(SDAR sdar PATHS $ENV{SDCCDIR}/bin REQUIRED)
message("Found sdar at ${SDAR}")

set(RGB2SDAS_PY ${HUGEDRIVER_SOURCE_DIR}/tools/rgb2sdas.py)

# We have to compile a .asm file into a .lib static library file.
# Here are the files from source, to intermediate files, to the .lib file
#
# The ASM file is in rgbasm's assembly languague, and we'll use rgbasm to
# compile it, then convert it to a library that sdar can use
set(HUGEDRIVER_ASM_SRC ${HUGEDRIVER_SOURCE_DIR}/hUGEDriver.asm)
set(HUGEDRIVER_OBJ ${HUGEDRIVER_BINARY_DIR}/hUGEDriver.obj)
set(HUGEDRIVER_O ${HUGEDRIVER_BINARY_DIR}/hUGEDRIVER.o)
set(HUGEDRIVER_LIB ${HUGEDRIVER_BINARY_DIR}/hUGEDriver.lib)

add_custom_command(OUTPUT ${HUGEDRIVER_OBJ}
  COMMAND ${RGBASM} -i${HUGEDRIVER_SOURCE_DIR} -DGBDK -o${HUGEDRIVER_OBJ} ${HUGEDRIVER_ASM_SRC}
)

add_custom_command(OUTPUT ${HUGEDRIVER_O}
  COMMAND ${Python_EXECUTABLE} ${RGB2SDAS_PY} -b 0 -o${HUGEDRIVER_O} ${HUGEDRIVER_OBJ}
  DEPENDS ${HUGEDRIVER_OBJ}
)

add_custom_command(OUTPUT ${HUGEDRIVER_LIB}
  COMMAND ${SDAR} -ru ${HUGEDRIVER_LIB} ${HUGEDRIVER_O}
  DEPENDS ${HUGEDRIVER_O}
)
add_custom_target(HugeDriver_Lib DEPENDS ${HUGEDRIVER_LIB})

add_library(hUGEDriver STATIC IMPORTED)
set_property(TARGET hUGEDriver PROPERTY
             IMPORTED_LOCATION ${HUGEDRIVER_LIB})
add_dependencies(hUGEDriver HugeDriver_Lib)

set(HUGEDRIVER_INCLUDE_DIR ${HUGEDRIVER_SOURCE_DIR}/include)
