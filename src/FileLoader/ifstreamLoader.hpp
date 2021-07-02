#ifndef FILE_LOADER_HPP
#define FILE_LOADER_HPP

#include <fstream>
#include <string>
#include <vector>

#include "ifstreamLoaderContainer.hpp"
#include "../Base/Core.hpp"

class ifstreamLoader
{
    inline static ifstreamLoader* instance = nullptr;

    std::vector<VulkanGame::Scope<ifstreamLoaderContainer>> filesLoaded;

    void throwOnNotFound (const std::ifstream& file, const std::string_view& fileNamePatch);

    template <typename... ARGS> std::string add (const ARGS&... args) { return (std::string (args) + ...); }

    std::string_view FileNameFromPatch (const std::string_view& patchFileName);

   public:
    static ifstreamLoader* getInstance( )
    {
        if (!instance) instance = new ifstreamLoader;
        return instance;
    }

    ~ifstreamLoader( );

    void LoadFileText (const std::string_view fileNamePatch);
    void LoadFileBin (const std::string_view fileNamePatch);

    std::vector<std::string> getFileTextLines (const std::string_view& fileName);
    std::vector<char>        readChars (const std::string_view& fileName);

    void closeFiles( );
    void closeFileByName (const std::string_view& fileName);
};

#endif  // FILE_LOADER_HPP
