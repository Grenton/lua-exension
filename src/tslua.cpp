#include "drivers/os/LinuxOS.h"
#include "drivers/console/StdConsole.h"
#include "luacore/TSInterpreter.hpp"
#include "luacore/StandaloneInterpreter.h"
#include <iostream>
#include <pthread.h>
#include <string>

#include "base.hpp"
#include "container.hpp"
#include "event.hpp"
#include "dispatcher.hpp"
#include "dout.hpp"

#include "drivers/ILogger.h"

pthread_cond_t m_main_cv;

luacore::ITSInterpreter *g_lc{};

#define LUA_QL(x) "'" x "'"

// clang-format off
const char* usageHint =   
    "usage: %s [options] [script [args]].\n"
    "Available options are:\n"
    "  -e stat  execute string " LUA_QL("stat") "\n"
    "  -l name  require library " LUA_QL("name") "\n"
    "  -i       enter interactive mode after executing " LUA_QL("script") "\n"
    "  -v       show version information\n"
    "  --       stop handling options\n"
    "  -        execute stdin and stop handling options\n";
// clang-format on

IOS *g_os{};

// We must add new board to clu core for native_linux
// Function was added here to compile code (should not be here)
IOS *BoardAPI_GetOS(unsigned int index) {
    return g_os;
}

ILogger *BoardAPI_GetLogger(unsigned int) {
    return nullptr;
}

int main(int argc, char **argv) {
    LinuxOS os{};
    g_os = &os;
    StdConsole console{};
    luacore::TSInterpreter lc{&os};
    luacore::StandaloneInterpreter sai{&lc, &console};
    g_lc = &lc;

    addBaseLib(lc.getState());
    addContainerLib(lc.getState());
    addEventLib(lc.getState());
    addDispatcherLib(lc.getState());

    if (argc == 1) {
        sai.loop();
    } else if (argc == 2) {
        std::string arg1 = argv[1];
        if (arg1 == "-v" || arg1 == "--version") {
            std::string version = LUA_COPYRIGHT;
            std::cout << version << std::endl;
        } else if (arg1 == "-h" || arg1 == "--help") {
            std::cout << usageHint << std::endl;
        } else {
            auto ret = lc.evaluate("dofile[[" + arg1 + "]]", nullptr);
            if (ret.size() > 0) {
                std::cout << ret << std::endl;
            }
        }
    } else {
        std::cout << usageHint << std::endl;
    }
    return 0;
}
