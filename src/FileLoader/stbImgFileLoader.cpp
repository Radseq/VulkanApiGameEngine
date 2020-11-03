#include "stbImgFileLoader.hpp"

#ifndef STB_IMAGE_IMPLEMENTATION
#    define STB_IMAGE_IMPLEMENTATION
#    include "stb_image.h"
#endif

GameCore::ImageContainer stbImgFileLoader::LoadFile (const std::string& fileName)
{
    GameCore::ImageContainer result;
    int            texWidth, texHeight, texChannels;

    char const* source = fileName.data( );

    stbi_uc* pixels = nullptr;
    pixels          = stbi_load (source, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

    if (!pixels) { throw std::runtime_error ("failed to load texture image!"); }

    vk::Extent3D textureSize {GameCore::util::to_uint_32_t (texWidth), GameCore::util::to_uint_32_t (texHeight), 1};

    vk::DeviceSize imageSize {0};  //= static_cast<vk::DeviceSize>(texWidth * texHeight * 4);
    imageSize = static_cast<vk::DeviceSize> (texWidth);
    imageSize *= static_cast<vk::DeviceSize> (texHeight);
    imageSize *= 4;

    result.TextureData  = pixels;
    //result.image.extent = textureSize;

    result.TextureExtend = textureSize;
    result.TextureExtend = textureSize;

    result.TextureSize  = imageSize;

    return result;
}
