#include "pch.h"
#include "ifstreamLoader.hpp"

void ifstreamLoader::throwOnNotFound (const std::ifstream& file, const std::string_view& fileNamePatch)
{
    if (!file.is_open( )) { throw std::runtime_error ("failed to open file!: " + std::string (fileNamePatch)); }
}

std::string_view ifstreamLoader::FileNameFromPatch (const std::string_view& patchFileName)
{
    return patchFileName.substr (patchFileName.find_last_of ("/") + 1);
}

ifstreamLoader::~ifstreamLoader( )
{
    closeFiles( );

    // for (auto* file : filesLoaded) { delete file; };

    // for (size_t i = 0; i < filesLoaded.size( ); i++) { delete filesLoaded [i]; }
}

void ifstreamLoader::LoadFileText (const std::string_view fileNamePatch)
{
    VulkanGame::Scope<ifstreamLoaderContainer> fileContainer = VulkanGame::CreateScope<ifstreamLoaderContainer>( );

    auto stringFilePatchName = static_cast<std::string> (fileNamePatch);

    fileContainer->file.open (stringFilePatchName, std::ios::in);
    fileContainer->fileName  = FileNameFromPatch (fileNamePatch);
    fileContainer->filePatch = fileNamePatch;
    fileContainer->loadType  = ifstreamLoaderFlags::text;

    throwOnNotFound (fileContainer->file, fileNamePatch);

    filesLoaded.push_back (std::move (fileContainer));
}

void ifstreamLoader::LoadFileBin (const std::string_view fileNamePatch)
{
    VulkanGame::Scope<ifstreamLoaderContainer> fileContainer = VulkanGame::CreateScope<ifstreamLoaderContainer>( );

    auto stringFilePatchName = static_cast<std::string> (fileNamePatch);

    fileContainer->file.open (stringFilePatchName, std::ios::ate | std::ios::binary);
    fileContainer->fileName  = FileNameFromPatch (fileNamePatch);
    fileContainer->filePatch = fileNamePatch;
    fileContainer->loadType  = ifstreamLoaderFlags::bin;

    throwOnNotFound (fileContainer->file, fileNamePatch);

    filesLoaded.push_back (std::move (fileContainer));
}

std::vector<std::string> ifstreamLoader::getFileTextLines (const std::string_view& fileName)
{
    std::vector<std::string> lines;

    for (auto& fileLoad : filesLoaded)
    {
        if (fileLoad->fileName == FileNameFromPatch (fileName))
        {
            for (std::string line; std::getline (fileLoad->file, line);) { lines.push_back (line); }
        }
    }

    return lines;
}

void ifstreamLoader::closeFiles( )
{
    for (size_t i = 0; i < filesLoaded.size( ); i++)
    {
        if (filesLoaded [i]->file.is_open( ))
        {
            filesLoaded [i]->file.close( );

            filesLoaded.erase (std::remove (filesLoaded.begin( ), filesLoaded.end( ), filesLoaded [i]),
                               filesLoaded.end( ));
        }
    }
}

void ifstreamLoader::closeFileByName (const std::string_view& fileName)
{
    for (auto& fileLoad : filesLoaded)
    {
        if (fileLoad->file.is_open( ))
        {
            fileLoad->file.close( );

            filesLoaded.erase (std::remove (filesLoaded.begin( ), filesLoaded.end( ), fileLoad), filesLoaded.end( ));
        }
    }
}

std::vector<char> ifstreamLoader::readChars (const std::string_view& fileName)
{
    std::vector<char> buffer;

    for (auto& fileLoad : filesLoaded)
    {
        size_t fileSize = static_cast<size_t> (fileLoad->file.tellg( ));
        buffer.resize (fileSize);

        fileLoad->file.seekg (0);
        fileLoad->file.read (buffer.data( ), fileSize);

        return buffer;
    }
    return buffer;
}