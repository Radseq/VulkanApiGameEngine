#include "pch.h"
#ifndef MAIN_HPP
#define MAIN_HPP

#include "Configuration.hpp"
#include "Game.hpp"
#include "ResourcePatch.hpp"

void SetPatch (const std::string_view& execPatch)
{
    std::string patch = execPatch.data( );
    std::replace (patch.begin( ), patch.end( ), '\\', '/');

    ResourcePatch::GetInstance( )->SetPatch (patch + "/../", "MainPatch");
    ResourcePatch::GetInstance( )->SetPatch (patch + "/../data", "DataPatch");
}

int main (int argc, char* argv [])
{
    SetPatch (argv [0]);
    LogMsg ("ENTER APPLICATION");

    try
    {
        Configuration::getInstance( )->setExePatch (argv [0]);
        Configuration::getInstance( )->loadFile ("Configuration.cfg");

        Game game;
        game.run( );
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what( ) << std::endl;
        LogMsg (e.what( ), LogLevel::LOG_ERROR);
        return EXIT_FAILURE;
    }

    LogMsg ("LEAVE APPLICATION");

    //_CrtDumpMemoryLeaks();
    return EXIT_SUCCESS;
}
#endif  // MAIN_HPP
