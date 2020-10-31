#ifndef TEXTURE2D_ARRAY_HPP
#define TEXTURE2D_ARRAY_HPP

#include <ktxvulkan.h>

#include <Buffer\CoreBuffeManager.hpp>
#include <Image\Image.hpp>
#include <Image\ImageManager.hpp>

#include "../FileLoader/KtxTextureLoader.hpp"
#include "Util/vkHelpers.hpp"

class Texture2DArray {
    const GameCore::VulkanDevice& context;

    void CreateImage (GameCore::Image& result, const ImageContainer& imgContainer);

    void allocateImgMemory (GameCore::Image& imageResult, const vk::MemoryPropertyFlags& properties) const;

   public:
    Texture2DArray (const GameCore::VulkanDevice& Context);
    void LoadTexture (GameCore::Image& result, const std::string& filePatch, const vk::Format& format);
};

#endif  // TEXTURE2D_ARRAY_HPP
