#include "Texture2D.hpp"

void Texture2D::CreateImage (GameCore::Image& result, const GameCore::VulkanDevice& context,
                             const ImageContainer& imgContainer, const vk::Format& format)
{
    GameCore::CoreBufferManager stagingBufferManager {context};
    GameCore::CoreBuffer        stagingBuffer;
    stagingBufferManager.createStagingBuffer (stagingBuffer, imgContainer.TextureSize, imgContainer.TextureData);

    GameCore::ImageManager imageManager {context};
    imageManager.CreateImage (result, imgContainer.TextureExtend, format, vk::ImageTiling::eOptimal,
                              vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
                              vk::MemoryPropertyFlagBits::eDeviceLocal, result.mipLevels, vk::SampleCountFlagBits::e1);

    imageManager.copyBufferToImage (stagingBuffer.getBuffer( ), result);

    stagingBuffer.destroy (context);

    // imageManager.generateMipmaps (texture);

    imageManager.createImageView (result, vk::ImageAspectFlagBits::eColor);
    imageManager.createSampler (result);
}
