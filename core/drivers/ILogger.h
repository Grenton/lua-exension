#pragma once

#include <cstdarg>

enum LogSeverity { DEBUG, TRACE, INFO, WARNING, ERROR, FATAL };
enum LogFacility { APP = (1 << 0), LUA = (1 << 1) };

struct ILogger {
    virtual ~ILogger() = default;

    virtual void setLogSeverity(LogSeverity severity, uint32_t facilityMask) = 0;
    virtual void log(LogFacility facility, LogSeverity severity, const char *fileName, int lineNum, const char *format,
                     va_list arg) = 0;
    virtual void logPacket(LogFacility facility, const uint8_t *buffer, int buffer_size) = 0;
};