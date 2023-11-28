#pragma once
#include <string>
#include <vector>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#include "LuaBridge/LuaBridge.h"
#pragma GCC diagnostic warning "-Wunused-but-set-variable"

class Base {
  public:
    Base(const std::string &name, const std::string &uuid, Base *base, lua_State *L);
    std::string getName() const;
    virtual std::string getType() const;
    std::string getUUID() const;
    Base *getParent() const;

    void addTag(const std::string &tag);
    void removeTag(const std::string &tag);
    int getTags(lua_State *L);
    bool checkIfTag(const std::string &tag);
    bool equals(lua_State *L);
    bool notequals(lua_State *L);

  private:
    std::string m_name;
    std::string m_uuid;
    Base *m_base;
    std::vector<std::string> m_tags;
};

void addBaseLib(lua_State *L);