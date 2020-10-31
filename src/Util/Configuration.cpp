#include "Configuration.hpp"

void Configuration::loadFile (const std::string& fileName) {
    ifstreamLoader::getInstance( )->LoadFileText (exePatch + "/../", fileName);

    std::vector<std::string> getFileTextLines = ifstreamLoader::getInstance( )->getFileTextLines (fileName);

    try {
                for each (const std::string& line in getFileTextLines) {
                        std::istringstream is_line (line);
                        std::string        key;
                        if (std::getline (is_line, key, '=')) {
                            key.erase (std::remove_if (key.begin( ), key.end( ), std::isspace), key.end( ));
                            std::string value;
                            if (std::getline (is_line, value)) { options [key] = value; }
                        }
                    }

                ifstreamLoader::getInstance( )->closeFileByName (fileName);
    } catch (const std::exception& e) {
        std::cerr << e.what( ) << std::endl;
        LogMsg (e.what( ));
    }
}

void Configuration::setExePatch (const std::string& ExePatch) { this->exePatch = ExePatch; }

int Configuration::GetConfigIntOption (const std::string& key) {
    int value = 0;
    try {
        value = std::stoi (GetConfigStringOption (key));
    } catch (const std::exception& e) { throw "Cant Find Configuration int option: " + key + " excp = " + e.what( ); }
    return value;
}

bool Configuration::GetConfigBoolOption (const std::string& key) {
    std::string value = GetConfigStringOption (key);

    value.erase (std::remove_if (value.begin( ), value.end( ), ::isspace), value.end( ));
    return value == "true";
}

const std::string Configuration::GetConfigStringOption (const std::string& key) {
    std::string result = "";

    auto find = options.find (key);

    if (find != options.end( )) {
        result = find->second;
    } else {
        throw "Cant Find Configuration option: " + key;
    }

    return result;
}
