#pragma once

#include "drivers/IConsole.h"
#include "luacore/ITSInterpreter.hpp"

namespace luacore {

class StandaloneInterpreter final {

  public:
    StandaloneInterpreter(ITSInterpreter *lua, IConsole *console);
    ~StandaloneInterpreter() = default;

    int loop();

  private:
    ITSInterpreter *lua{};
    IConsole *console{};
};

} // namespace luacore
