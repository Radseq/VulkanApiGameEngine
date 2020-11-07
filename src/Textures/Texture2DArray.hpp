#ifndef TEXTURE2D_ARRAY_HPP
#define TEXTURE2D_ARRAY_HPP

#include <ktxvulkan.h>

#include <Buffer\CoreBuffeManager.hpp>
#include <Image\Image.hpp>
#include <Image\ImageManager.hpp>

#include "../FileLoader/KtxTextureLoader.hpp"
#include "Util/vkHelpers.hpp"

class Texture2DArray
{
    const GraphicCore::VulkanDevice& context;

    void allocateImgMemory (GraphicCore::Image& imageResult, const vk::MemoryPropertyFlags& properties) const;

   public:
    Texture2DArray (const GraphicCore::VulkanDevice& Context);
    void LoadTexture (GraphicCore::Image& result, const std::string& filePatch, const vk::Format& format);
};

#endif  // TEXTURE2D_ARRAY_HPP
