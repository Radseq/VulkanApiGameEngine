#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP

#include <CoreBuffeManager.hpp>
#include <CoreImageView.hpp>
#include <CoreImage.hpp>
#include <ImageManager.hpp>

#include "KtxTextureLoader.hpp"
#include "pch.h"

class Texture2D
{
   public:
    VulkanGame::Ref<GraphicCore::CoreImage> CreateImage (const GraphicCore::VulkanDevice&   context,
                                                         const GraphicCore::ImageContainer& imgContainer,
                                                         const vk::Format&                  format);
};

#endif  // TEXTURE2D_HPP
