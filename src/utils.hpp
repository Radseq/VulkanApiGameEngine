#ifndef UTIL_HPP
#define UTIL_HPP

#include "ResourcePatch.hpp"

enum class LogLevel
{
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
};

void        LogMsg (const std::string& logMsg, const LogLevel& level = LogLevel::LOG_INFO);
std::string getCurrentDateTime (const std::string& note);
void        removeSpace (std::string& val);

#endif  // UTIL_HPP
