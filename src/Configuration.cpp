#include "pch.h"
#include "Configuration.hpp"

void Configuration::loadFile (const std::string& fileName)
{
    ifstreamLoader::getInstance( )->LoadFileText (exePatch + "/../" + fileName);

    std::vector<std::string> getFileTextLines = ifstreamLoader::getInstance( )->getFileTextLines (fileName);

    try
    {
        for (const auto& line : getFileTextLines)
        {
            std::istringstream is_line (line);
            std::string        key;
            if (std::getline (is_line, key, '='))
            {
                removeSpace (key);

                std::string value;
                if (std::getline (is_line, value))
                {
                    removeSpace (value);
                    options [key] = value;
                }
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what( ) << std::endl;
        LogMsg (e.what( ), LogLevel::LOG_ERROR);
    }

    ifstreamLoader::getInstance( )->closeFileByName (fileName);
}

void Configuration::setExePatch (const std::string& ExePatch) { this->exePatch = ExePatch; }

int Configuration::GetConfigIntOption (const std::string& key) const
{
    int value = 0;
    try
    {
        value = std::stoi (GetConfigStringOption (key));
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error ("Cant Find Configuration int option: " + key + " excp = " + e.what( ));
    }
    return value;
}

bool Configuration::GetConfigBoolOption (const std::string& key) const
{
    std::string value = GetConfigStringOption (key);

    return value == "true";
}

std::string Configuration::GetConfigStringOption (const std::string& key) const
{
    std::string result;

    auto find = options.find (key);

    if (find != options.end( )) { result = find->second; }
    else
    {
        throw std::runtime_error ("Cant Find Configuration option: " + key);
    }

    return result;
}
