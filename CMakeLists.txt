cmake_minimum_required(VERSION 3.13)

project(lua_test VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/top.cmake)
fetch_dependencies(${CMAKE_CURRENT_SOURCE_DIR}/cmake/revision.json)

include(FetchContent)

FetchContent_Declare(
    json
    SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/external/json
    URL               https://github.com/nlohmann/json/releases/download/v3.10.4/json.tar.xz
)

FetchContent_Populate(json)

add_repository(lua)
add_repository(luaBridge)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lua
    ${CMAKE_CURRENT_SOURCE_DIR}/luaBridge/Distribution
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/core/drivers/
    ${json_SOURCE_DIR}/include

    # includes from sources are not needed
    ${CMAKE_CURRENT_SOURCE_DIR}/src/classes
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

add_library(capi
    OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lapi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lauxlib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lbaselib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lcode.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lcorolib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lctype.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/ldblib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/ldebug.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/ldo.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/ldump.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/linit.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/liolib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lfunc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lgc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/llex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lmem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/loadlib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lopcodes.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/loslib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lparser.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lstate.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lstring.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lstrlib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/ltable.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/ltablib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/ltm.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lundump.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lutf8lib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lvm.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lzio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lmathlib.c
)

add_library(luacore
    OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/src/luacore/TSInterpreter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/luacore/Function.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/luacore/StandaloneInterpreter.cpp
)

add_library(luaclasses
    OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/src/classes/base.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/classes/container.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/classes/helper.cpp
)

add_executable(lua
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lua_interpreter.c)

add_executable(tslua
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tslua.cpp

    # classes supported only by ts interpreter
    ${CMAKE_CURRENT_SOURCE_DIR}/src/classes/event.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/classes/dispatcher.cpp
)

target_link_libraries(lua
    PRIVATE
    capi
    luaclasses
)

target_link_libraries(tslua
    PRIVATE
    capi
    luacore
    luaclasses
    -pthread
    -lrt
)