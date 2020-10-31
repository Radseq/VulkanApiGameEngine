#ifndef KTX_TEXTURE_LOADER_HPP
#define KTX_TEXTURE_LOADER_HPP

#include "../Containers/ImageContainer.hpp"

class KtxTextureLoader {
   public:
    ImageContainer LoadFile (const std::string& fileName);
};

#endif  // KTX_TEXTURE_LOADER_HPP
