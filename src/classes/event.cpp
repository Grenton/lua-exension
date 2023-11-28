#include "event.hpp"
#include "helper.hpp"

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

#include <list>
#include <memory>
#include "luacore/Function.hpp"
#include "luacore/ITSInterpreter.hpp"
#include "LuaBridge/LuaBridge.h"

#include <sys/time.h>

// Can be done better - now use extern for simplify
extern luacore::ITSInterpreter *g_lc;

uint64_t currentTimestamp() {
    struct timeval te;
    gettimeofday(&te, NULL);
    uint64_t milisecondsFromSeconds = te.tv_sec * 1000;
    uint64_t milisecondsFromMicroseconds = te.tv_usec / 1000;
    uint64_t milliseconds = milisecondsFromSeconds + milisecondsFromMicroseconds;
    return milliseconds;
}

static int __luaSleep(lua_State *L, int status, lua_KContext ctx) {
    uint64_t tmstp = currentTimestamp();
    uint64_t *end = (uint64_t *)ctx;
    if (tmstp < *end) {
        return lua_yieldk(L, 0, (lua_KContext)end, __luaSleep);
    } else {
        delete end;
        return 0;
    }
}

static int luaSleep(lua_State *L) {
    int ms = luaL_checkinteger(L, 1);
    uint64_t tmstp = currentTimestamp();
    uint64_t *end = new uint64_t(tmstp + ms);
    return lua_yieldk(L, 0, (lua_KContext)end, __luaSleep);
}

namespace luacore {
template <> void pushVariable<Base *>(lua_State *L, Base *x) {
    std::error_code ec;
    auto ret = luabridge::push(L, x, ec);
    (void)ret;
};
} // namespace luacore

Event::Event(const std::string &name, const std::string &uuid, Base *parent, lua_State *L)
    : Base(name, uuid, parent, L) {}

int Event::addCallback(lua_State *L) {
    int top = lua_gettop(L);
    if (top != 2) {
        lua_pushstring(L, "Invalid number of parameters");
        lua_error(L);
        return 1;
    }

    int ref = luaL_ref(L, LUA_REGISTRYINDEX);
    auto it = std::find(std::begin(this->handlers), std::end(handlers), ref);

    if (it == std::end(this->handlers)) {
        this->handlers.push_back(ref);
    }
    return 0;
}

int Event::deleteCallback(lua_State *L) {
    int ref = luaL_ref(L, LUA_REGISTRYINDEX);
    auto it = std::find(std::begin(this->handlers), std::end(handlers), ref);

    if (it != std::end(this->handlers)) {
        this->handlers.erase(it);
    }
    return 0;
}

void Event::operator()() {
    for (auto &han : handlers) {
        auto tsFunction = std::make_shared<luacore::Function<Base *>>(han, getParent());
        g_lc->execute(tsFunction);
    }
}

void addEventLib(lua_State *L) {
    lua_pushcfunction(L, luaSleep);
    lua_setglobal(L, "msleep");

    luabridge::getGlobalNamespace(L)
        .deriveClass<Event, Base>("Event")
        .addConstructor<void (*)(const std::string &name, const std::string &uuid, Base *, lua_State *)>()
        .addFunction("addCallback", &Event::addCallback)
        .addFunction("deleteCallback", &Event::deleteCallback)
        .addFunction("__call", &Event::operator())
        .endClass();
}