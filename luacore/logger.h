#pragma once

#include <stdint.h>

#define LUACORE_SEVERITY_DEBUG 0
#define LUACORE_SEVERITY_INFO 1
#define LUACORE_SEVERITY_WARNING 2
#define LUACORE_SEVERITY_ERROR 3

#ifndef LUACORE_LOG_FUNC
#define LUACORE_LOG_FUNC printf
#endif

#ifndef LUACORE_TIME_US_FUNC
#define LUACORE_TIME_US_FUNC ([]() { return 0; })
#endif

#ifdef LUACORE_LOG_FULL_PATH
#define LUACORE_LOG_FMT(severity, fmt, ...)                                                                            \
    uint64_t curr_tmp = LUACORE_TIME_US_FUNC();                                                                        \
    LUACORE_LOG_FUNC("[%lu.%06lu] %s:%d:%s: " fmt "%s\n", (long unsigned)(curr_tmp / 1000000),                         \
                     (long unsigned)(curr_tmp) % 1000000, __FILE__, __LINE__, severity, __VA_ARGS__);
#else
#include <string.h>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LUACORE_LOG_FMT(severity, fmt, ...)                                                                            \
    uint64_t curr_tmp = LUACORE_TIME_US_FUNC();                                                                        \
    LUACORE_LOG_FUNC("[%lu.%06lu] %s:%d:%s: " fmt "%s\n", (long unsigned)(curr_tmp / 1000000),                         \
                     (long unsigned)(curr_tmp) % 1000000, __FILENAME__, __LINE__, severity, __VA_ARGS__);
#endif

#ifdef LUACORE_LOG_LEVEL
#if LUACORE_LOG_LEVEL <= LUACORE_SEVERITY_DEBUG
#define LUACORE_DEBUG_(fmt, ...)                                                                                       \
    do {                                                                                                               \
        LUACORE_LOG_FMT("debug", fmt, __VA_ARGS__)                                                                     \
    } while (0)
#define LUACORE_DEBUG(...) LUACORE_DEBUG_(__VA_ARGS__, "")
#else
#define LUACORE_DEBUG(fmt, ...)
#endif
#else
#define LUACORE_DEBUG(fmt, ...)
#endif

#ifdef LUACORE_LOG_LEVEL
#if LUACORE_LOG_LEVEL <= LUACORE_SEVERITY_INFO
#define LUACORE_INFO_(fmt, ...)                                                                                        \
    do {                                                                                                               \
        LUACORE_LOG_FMT("info", fmt, __VA_ARGS__)                                                                      \
    } while (0)

#define LUACORE_INFO(...) LUACORE_INFO_(__VA_ARGS__, "")
#else
#define LUACORE_INFO(fmt, ...)
#endif
#else
#define LUACORE_INFO(fmt, ...)
#endif

#ifdef LUACORE_LOG_LEVEL
#if LUACORE_LOG_LEVEL <= LUACORE_SEVERITY_WARNING
#define LUACORE_WARNING_(fmt, ...)                                                                                     \
    do {                                                                                                               \
        LUACORE_LOG_FMT("warning", fmt, __VA_ARGS__)                                                                   \
    } while (0)

#define LUACORE_WARNING(...) LUACORE_WARNING_(__VA_ARGS__, "")
#else
#define LUACORE_WARNING(fmt, ...)
#endif
#else
#define LUACORE_WARNING(fmt, ...)
#endif

#ifdef LUACORE_LOG_LEVEL
#if LUACORE_LOG_LEVEL <= LUACORE_SEVERITY_ERROR
#define LUACORE_ERROR_(fmt, ...)                                                                                       \
    do {                                                                                                               \
        LUACORE_LOG_FMT("error", fmt, __VA_ARGS__)                                                                     \
    } while (0)

#define LUACORE_ERROR(...) LUACORE_ERROR_(__VA_ARGS__, "")
#else
#define LUACORE_ERROR(fmt, ...)
#endif
#else
#define LUACORE_ERROR(fmt, ...)
#endif
