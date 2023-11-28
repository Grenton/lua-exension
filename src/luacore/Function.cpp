#include "luacore/Function.hpp"

namespace luacore {
template <> void pushVariable<int>(lua_State *L, int x) {
    lua_pushinteger(L, x);
};

template <> void pushVariable<float>(lua_State *L, float x) {
    lua_pushnumber(L, x);
};

template <> void pushVariable<std::string>(lua_State *L, std::string x) {
    lua_pushstring(L, x.c_str());
};
} // namespace luacore