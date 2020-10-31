#include "../Util/Configuration.hpp"
#include "Game.hpp"
#ifndef main_HPP
#    define main_HPP

int main (int argc, char* argv []) {
    LogMsg ("ENTER APPLICATION");

    try {
        Configuration::getInstance( )->setExePatch (argv [0]);
        Configuration::getInstance( )->loadFile ("Configuration.cfg");

        Game game;
        game.run( );
    } catch (const std::exception& e) {
        std::cerr << e.what( ) << std::endl;
        LogMsg (e.what( ));
        return EXIT_FAILURE;
    }

    LogMsg ("LEAVE APPLICATION");

    //_CrtDumpMemoryLeaks();
    return EXIT_SUCCESS;
}
#endif
