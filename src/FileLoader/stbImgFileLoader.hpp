#ifndef STB_IMG_FILE_LOADER_HPP
#define STB_IMG_FILE_LOADER_HPP

#include <Util\math.hpp>
#include <stdexcept>
#include <string>

#include "Image/ImageContainer.hpp"

class stbImgFileLoader {
   public:
    GameCore::ImageContainer LoadFile (const std::string& fileName);
};

#endif  // STB_IMG_FILE_LOADER_HPP