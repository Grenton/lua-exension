#include "base.hpp"
#include <iostream>

extern "C" {
#define lua_c
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "helper.hpp"
#include "container.hpp"

static Container sys{};

Base::Base(const std::string &name, const std::string &uuid, Base *base, lua_State *L)
    : m_name{name}, m_uuid{uuid}, m_base{base}, m_tags{} {

    auto top = lua_gettop(L);

    lua_getglobal(L, "system");
    auto ret = luabridge::Stack<Container>::isInstance(L, -1);
    if (!ret) {
        std::error_code ec{};
        auto ret = luabridge::push(L, sys, ec);
        (void)ret;
        lua_setglobal(L, "system");
    }
    sys.addObject(this);

    // restore lua stack
    lua_settop(L, top);
}

std::string Base::getName() const {
    return m_name;
}

std::string Base::getType() const {
    return "Base";
}

std::string Base::getUUID() const {
    return m_uuid;
}

Base *Base::getParent() const {
    return this->m_base;
}

void Base::addTag(const std::string &tag) {
    this->m_tags.push_back(tag);
}

void Base::removeTag(const std::string &tag) {
    auto it = std::find(this->m_tags.begin(), this->m_tags.end(), tag);
    if (it != this->m_tags.end()) {
        this->m_tags.erase(it);
    }
}

int Base::getTags(lua_State *L) {
    lua_newtable(L);

    for (unsigned int i = 0; i < this->m_tags.size(); i++) {
        std::error_code ec{};
        auto ret = luabridge::push(L, i + 1, ec);
        ret = luabridge::push(L, this->m_tags[i], ec);
        (void)ret;
        lua_settable(L, -3);
    }
    return 1;
}

bool Base::checkIfTag(const std::string &tag) {
    if (std::find(this->m_tags.begin(), this->m_tags.end(), tag) != this->m_tags.end()) {
        return true;
    } else {
        return false;
    }
}

bool Base::equals(lua_State *L) {
    auto top = lua_gettop(L);

    if (top != 2) {
        return false;
    }
    if (luabridge::isInstance<Base>(L, -1) && luabridge::isInstance<Base>(L, -2)) {
        Base *o1 = luabridge::get<Base *>(L, -1);
        Base *o2 = luabridge::get<Base *>(L, -2);
        if (o1 == o2) {
            return true;
        }
        return false;
    }

    return false;
}

bool Base::notequals(lua_State *L) {
    auto top = lua_gettop(L);

    if (top != 2) {
        return false;
    }
    if (luabridge::isInstance<Base>(L, -1) && luabridge::isInstance<Base>(L, -2)) {
        Base *o1 = luabridge::get<Base *>(L, -1);
        Base *o2 = luabridge::get<Base *>(L, -2);
        if (o1 == o2) {
            return false;
        }
        return true;
    }

    return true;
}

static int luaB_type(lua_State *L) {
    int t = lua_type(L, 1);

    if (t == LUA_TUSERDATA) {
        auto ret = luabridge::isInstance<Base>(L, -1);
        if (ret) {
            Base *obj = luabridge::get<Base *>(L, -1);
            lua_pushstring(L, obj->getType().c_str());
            return 1;
        }
    }

    luaL_argcheck(L, t != LUA_TNONE, 1, "value expected");
    lua_pushstring(L, lua_typename(L, t));
    return 1;
}

extern void add_dout_lib(lua_State *L);

void addBaseLib(lua_State *L) {
    luabridge::getGlobalNamespace(L).addFunction("type", &luaB_type);

    luabridge::getGlobalNamespace(L)
        .beginClass<Base>("Base")
        .addConstructor<void (*)(const std::string &, const std::string &, Base *, lua_State *)>()
        .addProperty("uuid", &Base::getUUID)
        .addProperty("name", &Base::getName)
        .addFunction("getName", &Base::getName)
        .addFunction("addTag", &Base::addTag)
        .addFunction("removeTag", &Base::removeTag)
        .addFunction("getTags", &Base::getTags)
        .addProperty("parent", &Base::getParent)
        .addFunction("getParent", &Base::getParent)
        .addFunction("__eq", &Base::equals)
        .addFunction("__ne", &Base::notequals)
        .endClass();

    // add_dout_lib(L);
}