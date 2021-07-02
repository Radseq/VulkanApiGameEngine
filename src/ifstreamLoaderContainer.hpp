#ifndef FILE_LOADER_CONTAINER_HPP
#define FILE_LOADER_CONTAINER_HPP

#include "pch.h"

enum class ifstreamLoaderFlags
{
    text,
    bin
};

class ifstreamLoaderContainer
{
   public:
    std::string         filePatch;
    std::string         fileName;
    ifstreamLoaderFlags loadType;
    std::ifstream       file;
};

#endif  // FILE_LOADER_CONTAINER_HPP
