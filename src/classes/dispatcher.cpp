#include "dispatcher.hpp"

bool registerEventHandler(lua_State *L) {
    auto top = lua_gettop(L);

    if (top != 2) {
        return false;
    }

    if (luabridge::isInstance<Event>(L, -2)) {
        Event *ev = luabridge::get<Event *>(L, -2);
        ev->addCallback(L);
    }
    return false;
}

bool triggerEvent(lua_State *L) {
    auto top = lua_gettop(L);

    if (top != 1) {
        return false;
    }

    if (luabridge::isInstance<Event>(L, -1)) {
        Event *ev = luabridge::get<Event *>(L, -1);
        ev->operator()();
        return true;
    }
    return false;
}

void addDispatcherLib(lua_State *L) {
    luabridge::getGlobalNamespace(L)
        .beginNamespace("dispatcher")
        .addFunction("register", &registerEventHandler)
        .addFunction("trigger", &triggerEvent);
}