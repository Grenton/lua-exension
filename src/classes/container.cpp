#include "container.hpp"
#include <iostream>

extern "C" {
#define lua_c
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "lua_interpreter.h"
}

#include "base.hpp"
#include "helper.hpp"
#include "LuaBridge/LuaBridge.h"

Container::Container() {}

Container::~Container() {}

void Container::addObject(Base *obj) {
    this->m_obj.push_back(obj);
}

void Container::removeObject(Base *obj) {
    auto it = std::find(this->m_obj.begin(), this->m_obj.end(), obj);
    if (it != this->m_obj.end()) {
        this->m_obj.erase(it);
    }
}

int Container::getObjects(lua_State *L) {
    lua_newtable(L);

    for (unsigned int i = 0; i < this->m_obj.size(); i++) {
        std::error_code ec{};
        auto ret = luabridge::push(L, i + 1, ec);
        ret = luabridge::push(L, this->m_obj[i], ec);
        (void)ret;
        lua_settable(L, -3);
    }
    return 1;
}

int Container::getByTag(lua_State *L) {
    const char *tag = luaL_checkstring(L, 2);

    unsigned int k = 0;
    lua_newtable(L);

    for (unsigned int i = 0; i < this->m_obj.size(); i++) {
        if (this->m_obj[i]->checkIfTag(tag)) {
            std::error_code ec{};
            auto ret = luabridge::push(L, k + 1, ec);
            ret = luabridge::push(L, this->m_obj[i], ec);
            (void)ret;
            lua_settable(L, -3);
            k++;
        }
    }
    return 1;
}

int Container::pairs(lua_State *L) {
    return this->getObjects(L);
}

void addContainerLib(lua_State *L) {
    luabridge::getGlobalNamespace(L)
        .beginClass<Container>("Container")
        .addConstructor<void (*)(void)>()
        .addFunction("addObject", &Container::addObject)
        .addFunction("removeObject", &Container::removeObject)
        .addFunction("getObjects", &Container::getObjects)
        .addFunction("getByTag", &Container::getByTag)
        .addFunction("__pairs", &Container::pairs)
        .endClass();
}