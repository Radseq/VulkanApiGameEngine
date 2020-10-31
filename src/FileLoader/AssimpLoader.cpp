#include "AssimpLoader.hpp"

void AssimpLoader::throwOnNoLoaded (const std::string& filename) {
    const std::string error = importer.GetErrorString( );
    throw std::runtime_error (error + "\n\nNot Found. patchFile: " + filename);
}

void AssimpLoader::loadFromFile (const std::string& filename, const int flags) {
    pScene = importer.ReadFile (filename, flags);

    if (!pScene) throwOnNoLoaded (filename);
}

const aiScene* AssimpLoader::GetLoaded( ) { return pScene; }