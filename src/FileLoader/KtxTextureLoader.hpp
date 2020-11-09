#ifndef KTX_TEXTURE_LOADER_HPP
#define KTX_TEXTURE_LOADER_HPP

#include <ktxvulkan.h>

#include "Image/ImageContainer.hpp"

class KtxTextureLoader
{
    ktxTexture* m_Texture;

   public:
    GraphicCore::ImageContainer LoadFile (const std::string& fileName);
    ktxTexture*                 GetKtxTexture( ) { return m_Texture; }

    ~KtxTextureLoader( ) { delete m_Texture; }
};

#endif  // KTX_TEXTURE_LOADER_HPP