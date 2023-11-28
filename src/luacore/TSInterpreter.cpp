#include "luacore/TSInterpreter.hpp"
#include "luacore/logger.h"
#include <algorithm>
#include <cassert>
#include <sstream>

extern "C" {
#include "lstate.h"
#include <lauxlib.h>
#include <lualib.h>
}

namespace luacore {

static int traceback(lua_State *L) {
    if (!lua_isstring(L, 1)) /* 'message' not a string? */
        return 1;            /* keep it intact */
    lua_getglobal(L, "debug");
    if (!lua_istable(L, -1)) {
        lua_pop(L, 1);
        return 1;
    }
    lua_getfield(L, -1, "traceback");
    if (!lua_isfunction(L, -1)) {
        lua_pop(L, 2);
        return 1;
    }
    lua_pushvalue(L, 1);   /* pass error message */
    lua_pushinteger(L, 2); /* skip this function and traceback */
    lua_call(L, 2, 1);     /* call debug.traceback */
    return 1;
}

static std::string getReadableResult(lua_State *L) {
    (void)traceback;

    std::string ret{};

    int n = lua_gettop(L); /* number of arguments */
    int i;
    lua_getglobal(L, "tostring");
    for (i = 1; i <= n; i++) {
        const char *s;
        size_t l;
        lua_pushvalue(L, -1); /* function to be called */
        lua_pushvalue(L, i);  /* value to print */
        lua_call(L, 1, 1);
        s = lua_tolstring(L, -1, &l); /* get result */
        if (s == nullptr) {
            return "";
        }
        if (i > 1) {
            ret += "\t";
        }
        ret += s;
        lua_pop(L, 1); /* pop result */
    }
    return ret;
}

TSInterpreter::TSInterpreter(IOS *os) : os{os}, L{luaL_newstate()} {
    assert(this->os);

    this->mutex = this->os->createMutex();
    assert(this->mutex);

    this->task = this->os->createTask();
    assert(this->task);

    this->yieldTimer = this->os->createPeriodicTimer();
    assert(this->yieldTimer);

    this->loopCondition = this->os->createCondition();
    assert(this->loopCondition);

    if (L == NULL) {
        assert(false);
    }

    luaL_openlibs(L);

    constexpr int yield_timer_period = 20;
    this->yieldTimer->start(yield_timer_period, [this]() -> void { this->loopCondition->notifyAll(); });

    this->task->start([this](ITask *t) {
        while (true) {
            this->loopCondition->wait();

            for (auto it = std::begin(this->coroutines); it != std::end(this->coroutines);) {
                auto &co = *it;
                if (*co.state == LuaCoroutineTask::State::INIT) {
                    co.lua = lua_newthread(L);
                    LOG_DEBUG(LogFacility::LUA, "[Thread %p] Started", co.lua);

                    int ref = luaL_ref(L, LUA_REGISTRYINDEX);
                    if (co.type == LuaCoroutineTask::Type::EXPRESSION) {
                        luaL_loadstring(co.lua, co.typeVal.expression.c_str());
                        LOG_INFO(LogFacility::LUA, "[Thread %p] Processing lua string command %s", co.lua,
                                 co.typeVal.expression.c_str());
                    } else if (co.type == LuaCoroutineTask::Type::REFERENCE) {
                        co.typeVal.clbk->loadref(co.lua);
                        LOG_INFO(LogFacility::LUA, "[Thread %p] Processing lua reference command %p", co.lua,
                                 co.typeVal.clbk.get());
                    }

                    co.ref = ref;
                    *co.state = LuaCoroutineTask::State::IN_PROGRESS;
                }

                if (*co.state == LuaCoroutineTask::State::IN_PROGRESS) {
                    bool isFinished{false};

                    int args = 0;

                    if (co.type == LuaCoroutineTask::Type::REFERENCE) {
                        args = co.typeVal.clbk->args();
                    }

                    int nres{};
                    auto ret = lua_resume(co.lua, L, args, &nres);
                    switch (ret) {
                    case 0: // finished
                    {
                        const int top{lua_gettop(co.lua)};
                        if (top >= 1) {
                            *co.result = getReadableResult(co.lua);
                        }
                        *co.state = LuaCoroutineTask::State::OK;
                        isFinished = true;
                        LOG_INFO(LogFacility::LUA, "[Thread %p] Success with result: %s", co.lua, co.result->c_str());
                        co.condition->notifyAll();
                    } break;

                    case 1: // yield
                        LOG_DEBUG(LogFacility::LUA, "[Thread %p] Yield", co.lua);
                        break;

                    default: // error
                    {
                        const int top{lua_gettop(co.lua)};
                        const char *err{};
                        size_t l;
                        if (top >= 1) {
                            // if error get only top message
                            err = lua_tolstring(co.lua, -1, &l);
                        }
                        luaL_traceback(co.lua, co.lua, err, 1);
                        const char *tb = lua_tolstring(co.lua, -1, &l);
                        *co.result = tb;
                        *co.state = LuaCoroutineTask::State::ERROR;
                        isFinished = true;
                        LOG_ERROR(LogFacility::LUA, "[Thread %p] Error with result: %s", co.lua, co.result->c_str());
                        co.condition->notifyAll();
                    } break;
                    }

                    if (isFinished) {
                        lua_settop(co.lua, 0);
                        luaL_unref(co.lua, LUA_REGISTRYINDEX, co.ref);
                        lua_gc(co.lua, LUA_GCCOLLECT, 0);
                        lua_gc(L, LUA_GCCOLLECT, 0);
                        // lua_close(co.lua);

                        this->mutex->lock();
                        LUACORE_DEBUG("Lua command removed from queue id: %p", co.lua);
                        this->coroutines.erase(it++);
                        this->mutex->unlock();
                    } else {
                        it++;
                    }
                }
            }
        }
    });
}

TSInterpreter::~TSInterpreter() {
    this->task->stop();
    lua_close(L);
}

lua_State *TSInterpreter::getState() {
    return L;
}

std::string TSInterpreter::evaluate(const std::string &expression, bool *isOk) {
    return executeCoroutine(expression, isOk, true);
}

std::string TSInterpreter::evaluate(std::shared_ptr<IFunction> __clbk, bool *isOk) {
    return executeCoroutine(__clbk, isOk, true);
}

void TSInterpreter::execute(const std::string &expression) {
    executeCoroutine(expression, nullptr, false);
}

void TSInterpreter::execute(std::shared_ptr<IFunction> __clbk) {
    executeCoroutine(__clbk, nullptr, false);
}

std::string TSInterpreter::executeCoroutine(const std::string &expression, bool *isOk, bool isBlocking) {
    std::string result{};

    auto condition = this->os->createCondition();
    auto resultPtr = std::make_shared<std::string>();
    auto statePtr = std::make_shared<LuaCoroutineTask::State>(LuaCoroutineTask::State::INIT);

    LuaCoroutineTask task{};
    task.condition = condition;
    task.result = resultPtr;
    task.state = statePtr;
    task.type = LuaCoroutineTask::Type::EXPRESSION;
    task.typeVal.expression = expression;

    this->mutex->lock();
    LOG_INFO(LogFacility::LUA, "Added string command %s", expression.c_str());
    this->coroutines.push_back(task);
    this->mutex->unlock();

    this->loopCondition->notifyAll();

    if (isBlocking) {
        condition->wait();

        result = *resultPtr;
        if (isOk) {
            *isOk = *statePtr == LuaCoroutineTask::State::OK;
        }
    }

    return result;
}

std::string TSInterpreter::executeCoroutine(std::shared_ptr<IFunction> clbk, bool *isOk, bool isBlocking) {
    std::string result{};

    auto condition = this->os->createCondition();
    auto resultPtr = std::make_shared<std::string>();
    auto statePtr = std::make_shared<LuaCoroutineTask::State>(LuaCoroutineTask::State::INIT);

    LuaCoroutineTask task{};
    task.condition = condition;
    task.result = resultPtr;
    task.state = statePtr;
    task.type = LuaCoroutineTask::Type::REFERENCE;
    task.typeVal.clbk = clbk;

    this->mutex->lock();
    LOG_INFO(LogFacility::LUA, "Added reference command %p", clbk.get());
    this->coroutines.push_back(task);
    this->mutex->unlock();

    this->loopCondition->notifyAll();

    if (isBlocking) {
        condition->wait();

        result = *resultPtr;
        if (isOk) {
            *isOk = *statePtr == LuaCoroutineTask::State::OK;
        }
    }

    return result;
}
} // namespace luacore