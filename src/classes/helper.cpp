#include "helper.hpp"

std::string getClassName(const char *fullFuncName) {
    std::string fullFuncNameStr(fullFuncName);
    size_t pos = fullFuncNameStr.find_last_of("::");
    if (pos == std::string::npos) {
        return "";
    }
    return fullFuncNameStr.substr(0, pos - 1);
}