#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP

#include <Buffer\CoreBuffeManager.hpp>
#include <Image\Image.hpp>
#include <Image\ImageManager.hpp>

#include "../Base/Core.hpp"
#include "../FileLoader/KtxTextureLoader.hpp"

class Texture2D
{
   public:
    VulkanGame::Ref<GraphicCore::Image> CreateImage (const GraphicCore::VulkanDevice&   context,
                                                     const GraphicCore::ImageContainer& imgContainer,
                                                     const vk::Format&                  format);
};

#endif  // TEXTURE2D_HPP
