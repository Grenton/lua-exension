#pragma once

#include "IIODevice.h"
#include <string>

struct IConsole : public IIODevice {
    ~IConsole() override = default;

    virtual const char *readLine() = 0;
    virtual void print(const char *fmt, ...) = 0;
};
