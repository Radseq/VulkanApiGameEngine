#include "KtxTextureLoader.hpp"

#include <ktxvulkan.h>

GameCore::ImageContainer KtxTextureLoader::LoadFile (const std::string& fileName) {
    GameCore::ImageContainer result;

    ktxResult   loadresult = KTX_SUCCESS;
    ktxTexture* texture;

    loadresult = ktxTexture_CreateFromNamedFile (fileName.c_str( ), KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &texture);

    if (loadresult != KTX_SUCCESS) throw std::runtime_error ("\n\nNot Found. patchFile: " + fileName);

    assert (texture && "file loaded but data is null");

    vk::Extent3D textureSize {texture->baseWidth, texture->baseHeight, 1};

    //result.image.mipLevels = texture->numLevels;
    //result.image.extent = textureSize;

    result.TextureData = ktxTexture_GetData (texture);
    result.TextureSize = ktxTexture_GetSize (texture);

    //result.image.layerCount = texture->numLayers;
    result.layerCount = texture->numLayers;
    result.mipLevels  = texture->numLevels;
    result.TextureExtend = textureSize;

    delete texture;

    return result;
}