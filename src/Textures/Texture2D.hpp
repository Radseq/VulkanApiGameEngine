#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP

#include <Buffer\CoreBuffeManager.hpp>
#include <Image\Image.hpp>
#include <Image\ImageManager.hpp>

#include "../FileLoader/KtxTextureLoader.hpp"

class Texture2D
{
   public:
    void CreateImage (GameCore::Image& result, const GameCore::VulkanDevice& context,
                      const ImageContainer& imgContainer, const vk::Format& format);
};

#endif  // TEXTURE2D_HPP
