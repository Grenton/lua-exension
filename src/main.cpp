

#include <iostream>
#include <string>
#include <memory>
#include <iostream>
#include <string_view>

extern "C" {
#define lua_c
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "lua_interpreter.h"
}

#include "base.hpp"
#include "container.hpp"
#include "dout.hpp"

static void l_message(const char *pname, const char *msg) {
    if (pname)
        lua_writestringerror("%s: ", pname);
    lua_writestringerror("%s\n", msg);
}

int main(int argc, char **argv) {
    lua_State *L = luaL_newstate(); /* create state */
    if (L == NULL) {
        l_message(argv[0], "cannot create state: not enough memory");
        return EXIT_FAILURE;
    }

    luaL_openlibs(L);
    addBaseLib(L);
    addContainerLib(L);

    interpreter(argc, argv, L);
    return 0;
}
