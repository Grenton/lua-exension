#pragma once

#include "luacore/IFunction.hpp"
#include <algorithm>
#include <iostream>

namespace luacore {
template <typename T> void pushVariable(lua_State *L, T x);

template <typename TupleT, std::size_t... Is>
void pushTupleManual(lua_State *L, const TupleT &tp, std::index_sequence<Is...>) {
    (pushVariable(L, std::get<Is>(tp)), ...);
}

template <typename TupleT, std::size_t TupSize = std::tuple_size<TupleT>{}>
void pushTupleOnStack(lua_State *L, const TupleT &tp) {
    pushTupleManual(L, tp, std::make_index_sequence<TupSize>{});
}
template <class... Args> class Function : public IFunction {
  public:
    Function(int ref, Args... values) : ref{ref} {
        this->tuple = std::tuple<Args...>(values...);
    }

    void loadref(lua_State *L) override {
        lua_rawgeti(L, LUA_REGISTRYINDEX, this->ref);
        pushTupleOnStack(L, this->tuple);
    }

    int args() const {
        return std::tuple_size<std::tuple<Args...>>{};
    }

  private:
    int ref;
    std::tuple<Args...> tuple;
};
} // namespace luacore