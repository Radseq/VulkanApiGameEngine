#pragma once

#include <fstream>
#include <string>

const std::string getAssetPath( );
const std::string getFilePath( );
const std::string logPatch = getFilePath( );

enum class LogLevel {
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
};

void        LogMsg (const std::string& logMsg, const LogLevel& level = LogLevel::LOG_INFO);
std::string getCurrentDateTime (const std::string& note);
