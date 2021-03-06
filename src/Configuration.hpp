#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include "ifstreamLoader.hpp"
#include "pch.h"
#include "utils.hpp"

class Configuration
{
    std::map<std::string, std::string> options;

    inline static Configuration* instance = nullptr;

    std::string exePatch;

   public:
    static Configuration* getInstance( )
    {
        if (!instance) instance = new Configuration;
        return instance;
    }

    // todo template <typename T>
    std::string GetConfigStringOption (const std::string& key) const;
    int         GetConfigIntOption (const std::string& key) const;
    bool        GetConfigBoolOption (const std::string& key) const;

    void loadFile (const std::string& fileName);

    void setExePatch (const std::string& ExePatch);
};

#endif  // CONFIGURATION_HPP
