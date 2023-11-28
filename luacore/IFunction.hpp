#pragma once

extern "C" {
#include <lua.h>
}

namespace luacore {
struct IFunction {
    virtual ~IFunction() = default;

    virtual void loadref(lua_State *L) = 0;
    virtual int args() const = 0;
};
} // namespace luacore