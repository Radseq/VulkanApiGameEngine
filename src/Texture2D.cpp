#include "pch.h"
#include "Texture2D.hpp"

VulkanGame::Ref<GraphicCore::CoreImage> Texture2D::CreateImage (const GraphicCore::VulkanDevice&   context,
                                                                const GraphicCore::ImageContainer& imgContainer,
                                                                const vk::Format&                  format)
{
    GraphicCore::CoreBufferManager stagingBufferManager {context};
    GraphicCore::CoreBuffer        stagingBuffer;
    stagingBufferManager.createStagingBuffer (stagingBuffer, imgContainer.TextureSize, imgContainer.TextureData);

    GraphicCore::ImageManager imageManager {context};

    auto result = VulkanGame::CreateRef<GraphicCore::CoreImage> (
        context, imgContainer.TextureExtend, format,
        vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, vk::SampleCountFlagBits::e1,
        imgContainer.mipLevels,
        imgContainer.arrayLayer);

    result->Crete (vk::MemoryPropertyFlagBits::eDeviceLocal);

    imageManager.copyBufferToImage (stagingBuffer.getBuffer( ), result);

    stagingBuffer.destroy (context);

    // imageManager.generateMipmaps (texture);

    VulkanGame::Ref<GraphicCore::CoreImageView> view =
        VulkanGame::CreateRef<GraphicCore::CoreImageView> (context.getVkDevice( ));
    view->CreateImageView (result, vk::ImageViewType::e2D);

    result->GetViews( ).insert (view);

    return result;
}
