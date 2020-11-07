#ifndef IMAGE_CONTAINER_HPP
#define IMAGE_CONTAINER_HPP

#include <vulkan/vulkan.hpp>

namespace GraphicCore
{
    class ImageContainer
    {
       public:
        void*        TextureData;  // someday rewrite to template
        size_t       TextureSize;  // someday rewrite to template
        vk::Extent3D TextureExtend;

        uint32_t mipLevels {1};

        uint32_t layerCount {1};
    };
}  // namespace GraphicCore
#endif  // IMAGE_CONTAINER_HPP
