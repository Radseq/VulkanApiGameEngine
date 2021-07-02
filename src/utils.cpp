#include "pch.h"
#include "utils.hpp"

std::string getCurrentDateTime (const std::string& note)
{
    time_t    now = time (nullptr);
    char      buf [80] { };
    struct tm timeStruct
    {
    };  //= localtime_s(&now);
    localtime_s (&timeStruct, &now);

    if (note == "now")
        strftime (buf, sizeof (buf), "%Y-%m-%d %X", &timeStruct);
    else if (note == "date")
        strftime (buf, sizeof (buf), "%Y-%m-%d", &timeStruct);
    return std::string (buf);
};

void LogMsg (const std::string& logMsg, const LogLevel& level)
{
    const std::string logPatch = ResourcePatch::GetInstance( )->GetPatch ("MainPatch") + "Log.txt";

    //+ getCurrentDateTime("date") + ".txt";
    const std::string now = getCurrentDateTime ("now");
    std::ofstream     ofs (logPatch.c_str( ), std::ios_base::out | std::ios_base::app);
    ofs << now << '\t' << logMsg << '\n';

    if (ofs.is_open( )) ofs.close( );
}

void removeSpace (std::string& val)
{
    val.erase (std::remove_if (val.begin( ), val.end( ), ::isspace), val.end( ));
}