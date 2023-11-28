#pragma once

#include "base.hpp"
#include <list>

class Event : public Base {
  public:
    Event(const std::string &name, const std::string &uuid, Base *parent, lua_State *L);

    int addCallback(lua_State *L);
    int deleteCallback(lua_State *L);
    void operator()();

  private:
    std::list<int> handlers{};
};

void addEventLib(lua_State *L);