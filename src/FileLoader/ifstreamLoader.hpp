#ifndef FILE_LOADER_HPP
#define FILE_LOADER_HPP

#include <fstream>
#include <string>
#include <vector>

#include "ifstreamLoaderContainer.hpp"

class ifstreamLoader {
    inline static ifstreamLoader* instance = nullptr;

    std::vector<ifstreamLoaderContainer*> filesLoaded;

    void throwOnNotFound (const std::ifstream& file, const std::string& fileNamePatch);

    ifstreamLoaderContainer* getContainerByFileName (const std::string& fileName);

   public:
    static ifstreamLoader* getInstance( ) {
        if (!instance) instance = new ifstreamLoader;
        return instance;
    }

    ~ifstreamLoader( );

    void LoadFileText (const std::string& patch, const std::string& fileName);
    void LoadFileBin (const std::string& patch, const std::string& fileName);

    std::vector<std::string> getFileTextLines (const std::string& fileName);
    std::vector<char>        readChars (const std::string& filename);

    void closeFiles( );
    void closeFileByName (const std::string& fileName);
};

#endif  // FILE_LOADER_HPP
