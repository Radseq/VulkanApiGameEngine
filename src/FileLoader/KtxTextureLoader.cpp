#include "KtxTextureLoader.hpp"

GraphicCore::ImageContainer KtxTextureLoader::LoadFile (const std::string& fileName)
{
    GraphicCore::ImageContainer result;

    ktxResult loadresult = KTX_SUCCESS;

    loadresult = ktxTexture_CreateFromNamedFile (fileName.c_str( ), KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &m_Texture);

    if (loadresult != KTX_SUCCESS) throw std::runtime_error ("\n\nNot Found. patchFile: " + fileName);

    assert (m_Texture && "file loaded but data is null");

    vk::Extent3D textureSize {m_Texture->baseWidth, m_Texture->baseHeight, 1};

    result.TextureData = ktxTexture_GetData (m_Texture);
    result.TextureSize = ktxTexture_GetSize (m_Texture);

    result.layerCount    = m_Texture->numLayers;
    result.mipLevels     = m_Texture->numLevels;
    result.TextureExtend = textureSize;

    return result;
}