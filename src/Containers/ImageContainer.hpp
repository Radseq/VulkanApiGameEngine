#ifndef IMAGE_LOADER_HPP
#define IMAGE_LOADER_HPP

#include <Image\Image.hpp>

class ImageContainer {
   public:
    void*        TextureData;  // someday rewrite to template
    size_t       TextureSize;  // someday rewrite to template
    vk::Extent3D TextureExtend;
   // uint32_t     mipLevels;
    //uint32_t     layerCount;
    //GameCore::Image image { };

    uint32_t mipLevels { };

    uint32_t layerCount { };
};

#endif  // KTX_TEXTURE_LOADER_HPP
