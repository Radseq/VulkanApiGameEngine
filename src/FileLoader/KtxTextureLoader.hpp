#ifndef KTX_TEXTURE_LOADER_HPP
#define KTX_TEXTURE_LOADER_HPP

#include "Image/ImageContainer.hpp"

class KtxTextureLoader {
   public:
    GraphicCore::ImageContainer LoadFile (const std::string& fileName);
};

#endif  // KTX_TEXTURE_LOADER_HPP
