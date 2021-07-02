#ifndef TEXTURE2D_ARRAY_HPP
#define TEXTURE2D_ARRAY_HPP

#include <ktxvulkan.h>

#include <CoreBuffeManager.hpp>
#include <CoreImageView.hpp>
#include <CoreImage.hpp>
#include <ImageManager.hpp>

#include "KtxTextureLoader.hpp"
#include "pch.h"
#include "vkHelpers.hpp"

class KtxTexture2DArray
{
    const GraphicCore::VulkanDevice& context;

   public:
    KtxTexture2DArray (const GraphicCore::VulkanDevice& Context);
    VulkanGame::Ref<GraphicCore::CoreImage> LoadTexture (const std::string& filePatch, const vk::Format& format);
};

#endif  // TEXTURE2D_ARRAY_HPP
