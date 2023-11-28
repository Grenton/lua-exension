#pragma once
#include <string>
#include <vector>

extern "C" {
#define lua_c
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "lua_interpreter.h"
}

#include "base.hpp"

class Container {
  public:
    Container(void);
    ~Container(void);

    void addObject(Base *obj);
    void removeObject(Base *obj);
    int getObjects(lua_State *L);
    int getByTag(lua_State *L);
    int pairs(lua_State *L);
    void check(void);

  private:
    std::vector<Base *> m_obj;
};

void addContainerLib(lua_State *L);