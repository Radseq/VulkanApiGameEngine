#ifndef UTIL_HPP
#define UTIL_HPP

#include <algorithm>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>

#include "../Base/ResourcePatch.hpp"

enum class LogLevel
{
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
};

void        LogMsg (const std::string& logMsg, const LogLevel& level = LogLevel::LOG_INFO);
std::string getCurrentDateTime (const std::string& note);

#endif  // UTIL_HPP
