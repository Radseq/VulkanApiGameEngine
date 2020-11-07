#include "stbImgFileLoader.hpp"

#ifndef STB_IMAGE_IMPLEMENTATION
#    define STB_IMAGE_IMPLEMENTATION
#    include "stb_image.h"
#endif

GraphicCore::ImageContainer stbImgFileLoader::LoadFile (const std::string& fileName)
{
    GraphicCore::ImageContainer result;

    int texWidth {0};
    int texHeight {0};
    int texChannels {0};

    char const* source = fileName.data( );

    stbi_uc* Data = nullptr;
    Data          = stbi_load (source, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

    if (!Data) { throw std::runtime_error ("failed to load texture image!"); }

    vk::Extent3D textureSize {GraphicCore::util::to_uint_32_t (texWidth), GraphicCore::util::to_uint_32_t (texHeight), 1};

    vk::DeviceSize imageSize {0};  //= static_cast<vk::DeviceSize>(texWidth * texHeight * 4);
    imageSize = static_cast<vk::DeviceSize> (texWidth);
    imageSize *= static_cast<vk::DeviceSize> (texHeight);
    imageSize *= 4;

    result.TextureData   = Data;
    result.TextureExtend = textureSize;
    result.TextureExtend = textureSize;

    result.TextureSize = imageSize;

    stbi_image_free (Data);

    return result;
}
