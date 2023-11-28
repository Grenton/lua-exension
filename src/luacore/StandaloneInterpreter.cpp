#include "luacore/StandaloneInterpreter.h"
#include <cassert>

namespace luacore {

StandaloneInterpreter::StandaloneInterpreter(ITSInterpreter *lua, IConsole *console) : lua{lua}, console{console} {
    assert(this->lua);
    assert(this->console);
}

int StandaloneInterpreter::loop() {
    this->console->print(LUA_COPYRIGHT "\n");
    while (true) {

        std::string input{};
        this->console->print("> ");

        while (true) {
            std::string tmp{this->console->readLine()};
            input += tmp;

            if (input.size() > 0) {
                auto req = "local __func, __err = load([[" + input + "]])  if(__err) then return __err end";
                auto res = this->lua->evaluate(req, nullptr);
                if (res.size() == 0) {
                    // no error - request is complete
                    auto result = this->lua->evaluate(input, nullptr);
                    if (result.size() > 0) {
                        this->console->print(result.c_str());
                        this->console->print("\n");
                    }
                    break;
                } else {
                    static const std::string eof = "<eof>";
                    if (res.size() >= eof.size()) {
                        if (res.compare(res.size() - eof.size(), eof.size(), "<eof>") == 0) {
                            // continuation
                            this->console->print(">>");
                        } else {
                            // error
                            this->console->print(res.c_str());
                            this->console->print("\n");
                            break;
                        }
                    } else {
                        break;
                    }
                }
            } else {
                break;
            }
        }
    }
    return 0;
}

} // namespace luacore
