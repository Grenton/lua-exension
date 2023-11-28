#pragma once
#include <string>

std::string getClassName(const char *fullFuncName);

#define __CLASS__ getClassName(__PRETTY_FUNCTION__)