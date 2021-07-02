#ifndef STB_IMG_FILE_LOADER_HPP
#define STB_IMG_FILE_LOADER_HPP

#include "ImageContainer.hpp"
#include "pch.h"

class stbImgFileLoader
{
   public:
    GraphicCore::ImageContainer LoadFile (const std::string& fileName);
};

#endif  // STB_IMG_FILE_LOADER_HPP