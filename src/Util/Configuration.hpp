#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>

#include "../FileLoader/ifstreamLoader.hpp"
#include "utils.hpp"

class Configuration {
    std::map<std::string, std::string> options;

    inline static Configuration* instance = nullptr;

    std::string exePatch = "";

   public:
    static Configuration* getInstance( ) {
        if (!instance) instance = new Configuration;
        return instance;
    }

    // todo template <typename T>
    const std::string GetConfigStringOption (const std::string& key);
    int               GetConfigIntOption (const std::string& key);
    bool              GetConfigBoolOption (const std::string& key);

    void loadFile (const std::string& fileName);

    void setExePatch (const std::string& ExePatch);
};

#endif  // CONFIGURATION_HPP
