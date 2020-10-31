#include "utils.hpp"

#include <algorithm>
#include <iostream>
#include <mutex>

std::string getCurrentDateTime (const std::string& note) {
    time_t    now = time (nullptr);
    char      buf [80] { };
    struct tm timeStruct { };  //= localtime_s(&now);
    localtime_s (&timeStruct, &now);

    if (note == "now")
        strftime (buf, sizeof (buf), "%Y-%m-%d %X", &timeStruct);
    else if (note == "date")
        strftime (buf, sizeof (buf), "%Y-%m-%d", &timeStruct);
    return std::string (buf);
};

void LogMsg (const std::string& logMsg, const LogLevel& level) {
    const std::string filePath = logPatch + "/../../" + "Log.txt";
    //+ getCurrentDateTime("date") + ".txt";
    const std::string now = getCurrentDateTime ("now");
    std::ofstream     ofs (filePath.c_str( ), std::ios_base::out | std::ios_base::app);
    ofs << now << '\t' << logMsg << '\n';

    if (ofs.is_open( )) ofs.close( );
}

const std::string getFilePath( ) {
    static std::string    path;
    static std::once_flag once;
    std::call_once (once, [] {
        std::string file (__FILE__);
        std::replace (file.begin( ), file.end( ), '\\', '/');
        const std::string::size_type lastSlash = file.rfind ('/');
        file                                   = file.substr (0, lastSlash);
        path                                   = file;
    });
    return path;
}

const std::string getAssetPath( ) {
    static std::string path;
    path = logPatch + "/../../data/";
    return path;
}
