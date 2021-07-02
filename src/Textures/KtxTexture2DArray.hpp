#ifndef TEXTURE2D_ARRAY_HPP
#define TEXTURE2D_ARRAY_HPP

#include <ktxvulkan.h>

#include <Buffer\CoreBuffeManager.hpp>
#include <Image\Image.hpp>
#include <Image\ImageManager.hpp>

#include "../FileLoader/KtxTextureLoader.hpp"
#include "Util/vkHelpers.hpp"
#include "../Base/Core.hpp"

class KtxTexture2DArray
{
    const GraphicCore::VulkanDevice& context;

   public:
    KtxTexture2DArray (const GraphicCore::VulkanDevice& Context);
    VulkanGame::Ref<GraphicCore::Image> LoadTexture (const std::string& filePatch,
                                                     const vk::Format& format);
};

#endif  // TEXTURE2D_ARRAY_HPP
