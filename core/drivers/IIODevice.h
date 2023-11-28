#pragma once

#include <cstdint>
#include <cstdlib>

struct IIODevice {
    virtual ~IIODevice() = default;

    virtual int writeSome(const uint8_t *data, int size, int timeout = 0) = 0;
    virtual int readSome(uint8_t *buff, int size, int timeout = 0) = 0;
    virtual int getAvailableBytes() const = 0;
};
