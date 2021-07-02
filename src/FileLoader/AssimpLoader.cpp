#include "AssimpLoader.hpp"

void AssimpLoader::throwOnNoLoaded (const std::string_view& filename)
{
    const std::string error = importer.GetErrorString( );
    throw std::runtime_error (error + "\n\nNot Found. patchFile: " + filename.data());
}

void AssimpLoader::loadFromFile (const std::string_view& filename, const int flags)
{
    pScene = importer.ReadFile (filename.data(), flags);

    if (!pScene) throwOnNoLoaded (filename);
}

const aiScene* AssimpLoader::GetLoaded( ) { return pScene; }