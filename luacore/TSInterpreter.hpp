#pragma once

#include "drivers/IOS.h"
#include "luacore/ITSInterpreter.hpp"
#include <list>
#include <map>
#include <memory>
#include <set>

extern "C" {
#include <lua.h>
}

namespace luacore {
class TSInterpreter : public ITSInterpreter {
  public:
    TSInterpreter(IOS *os);
    ~TSInterpreter() override;

    std::string evaluate(const std::string &expression, bool *isOk) override;
    void execute(const std::string &expression) override;

    std::string evaluate(std::shared_ptr<IFunction> __clbk, bool *isOk) override;
    void execute(std::shared_ptr<IFunction> __clbk) override;

    lua_State *getState();

  private:
    std::string executeCoroutine(const std::string &expression, bool *isOk, bool isBlocking);
    std::string executeCoroutine(std::shared_ptr<IFunction> __clbk, bool *isOk, bool isBlocking);

    IOS *os{};
    std::shared_ptr<IMutex> mutex{};
    lua_State *L{};

    struct LuaCoroutineTask {
        enum State { INIT, IN_PROGRESS, ERROR, OK };

        enum Type { EXPRESSION, REFERENCE };

        struct TypeVal {
            std::string expression;
            std::shared_ptr<IFunction> clbk;
        };

        lua_State *lua;
        std::shared_ptr<ICondition> condition;
        std::shared_ptr<std::string> result;
        std::shared_ptr<State> state{};
        Type type;
        TypeVal typeVal;
        int ref;

        bool operator==(const LuaCoroutineTask &task) {
            return this->lua == task.lua;
        }
    };

    std::list<LuaCoroutineTask> coroutines{};
    std::shared_ptr<ITask> task{};
    std::shared_ptr<ITimer> yieldTimer{};
    std::shared_ptr<ICondition> loopCondition{};
};
} // namespace luacore