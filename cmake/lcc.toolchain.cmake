if(NOT DEFINED ENV{GBDK_HOME})
    message(FATAL_ERROR "Environment variable GBDK_HOME must be set. See the README.md file.")
endif()

# Note that we don't reference SDCCDIR here, but the LCC compiler expects it to be set
if(NOT DEFINED ENV{SDCCDIR})
    message(FATAL_ERROR "Environment variable SDCCDIR must be set. See the README.md file.")
endif()

find_program(LCC lcc PATHS $ENV{GBDK_HOME}/bin REQUIRED)
message("Found LCC at ${LCC}")
if(LCC STREQUAL "LCC-NOTFOUND")
    message(FATAL_ERROR "Could not find lcc. Be sure it exists under $GBDK_HOME/bin")
endif()

set(CMAKE_SYSTEM_NAME Generic CACHE INTERNAL "")
set(CMAKE_SYSTEM_PROCESSOR lr35902 CACHE INTERNAL "")

set(CMAKE_C_FLAGS_INIT "")
set(CMAKE_EXE_LINKER_FLAGS_INIT "")

set(CMAKE_C_COMPILER ${LCC})

set(CMAKE_EXECUTABLE_SUFFIX_C .gb)
