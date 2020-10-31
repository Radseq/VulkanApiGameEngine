#include "ifstreamLoader.hpp"

void ifstreamLoader::throwOnNotFound (const std::ifstream& file, const std::string& fileNamePatch) {
    if (!file.is_open( )) { throw std::runtime_error ("failed to open file!: " + fileNamePatch); }
}

ifstreamLoaderContainer* ifstreamLoader::getContainerByFileName (const std::string& fileName) {
    for (size_t i = 0; i < filesLoaded.size( ); ++i) {
        if (filesLoaded [i]->fileName == fileName) { return filesLoaded [i]; }
    }
    return nullptr;
}

ifstreamLoader::~ifstreamLoader( ) {
    closeFiles( );

    for (size_t i = 0; i < filesLoaded.size( ); i++) { delete filesLoaded [i]; }
}

void ifstreamLoader::LoadFileText (const std::string& patch, const std::string& fileName) {
    ifstreamLoaderContainer* fileContainer = new ifstreamLoaderContainer( );
    fileContainer->file.open (patch + fileName, std::ios::in);
    fileContainer->fileName  = fileName;
    fileContainer->filePatch = patch;
    fileContainer->loadType  = ifstreamLoaderFlags::text;

    throwOnNotFound (fileContainer->file, patch + fileName);

    filesLoaded.push_back (fileContainer);
}

void ifstreamLoader::LoadFileBin (const std::string& patch, const std::string& fileName) {
    ifstreamLoaderContainer* fileContainer = new ifstreamLoaderContainer( );
    fileContainer->file.open (patch + fileName, std::ios::ate | std::ios::binary);
    fileContainer->fileName  = fileName;
    fileContainer->filePatch = patch;
    fileContainer->loadType  = ifstreamLoaderFlags::bin;

    throwOnNotFound (fileContainer->file, patch + fileName);

    filesLoaded.push_back (fileContainer);
}

std::vector<std::string> ifstreamLoader::getFileTextLines (const std::string& fileName) {
    std::vector<std::string> lines;

    auto* fileContainer = getContainerByFileName (fileName);

    for (std::string line; std::getline (fileContainer->file, line);) { lines.push_back (line); }

    return lines;
}

void ifstreamLoader::closeFiles( ) {
    for (size_t i = 0; i < filesLoaded.size( ); i++) {
        if (filesLoaded [i]->file.is_open( )) {
            filesLoaded [i]->file.close( );
            delete filesLoaded [i];

            filesLoaded.erase (std::remove (filesLoaded.begin( ), filesLoaded.end( ), filesLoaded [i]),
                               filesLoaded.end( ));
        }
    }
}

void ifstreamLoader::closeFileByName (const std::string& fileName) {
    auto* fileContainer = getContainerByFileName (fileName);
    if (fileContainer->file.is_open( )) {
        fileContainer->file.close( );

        delete fileContainer;

        filesLoaded.erase (std::remove (filesLoaded.begin( ), filesLoaded.end( ), fileContainer), filesLoaded.end( ));
    }
}

std::vector<char> ifstreamLoader::readChars (const std::string& filename) {
    auto* fileContainer = getContainerByFileName (filename);

    size_t            fileSize = (size_t) fileContainer->file.tellg( );
    std::vector<char> buffer (fileSize);

    fileContainer->file.seekg (0);
    fileContainer->file.read (buffer.data( ), fileSize);

    return buffer;
}