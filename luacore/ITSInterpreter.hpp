#pragma once

#include "luacore/IFunction.hpp"
#include <memory>
#include <string>

namespace luacore {
struct ITSInterpreter {
    virtual ~ITSInterpreter() = default;

    virtual std::string evaluate(const std::string &__expression, bool *__isOk) = 0;
    virtual void execute(const std::string &__expression) = 0;

    virtual std::string evaluate(std::shared_ptr<IFunction> __clbk, bool *__isOk) = 0;
    virtual void execute(std::shared_ptr<IFunction> __clbk) = 0;
};
} // namespace luacore