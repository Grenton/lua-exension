
message(STATUS "included ${CMAKE_CURRENT_LIST_FILE}")

set(CMAKE_BUILD_SYSTEM_DIR ${CMAKE_CURRENT_LIST_DIR})
include(${CMAKE_BUILD_SYSTEM_DIR}/macros.cmake)

if(PROJECT_IS_TOP_LEVEL)

    set(FETCHCONTENT_QUIET OFF)
    set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE BOOL "disconect external repos")
    set(CMAKE_VERBOSE_MAKEFILE FALSE CACHE BOOL "Choose makefile verbose" FORCE)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    if (UNIT_TESTS)
        set(OPTIMALIZATION_LEVEL -O0)
    else()
        set(OPTIMALIZATION_LEVEL -Og)
    endif()

    add_compile_options(
        $<$<COMPILE_LANGUAGE:C>:-std=gnu11>
        $<$<COMPILE_LANGUAGE:CXX>:-std=gnu++17>
        -Wall -Wno-psabi -Werror -Wno-error -ffunction-sections -fdata-sections -fno-common ${OPTIMALIZATION_LEVEL} -g -fstack-protector-strong -pthread -fexceptions
    )

    add_link_options(
        -Wl,-Map=output.map
        -static-libstdc++
    )

    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)

else()
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
endif()
