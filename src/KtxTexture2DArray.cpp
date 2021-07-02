#include "pch.h"
#include "KtxTexture2DArray.hpp"

KtxTexture2DArray::KtxTexture2DArray (const GraphicCore::VulkanDevice& Context)
    : context (Context)
{
}

VulkanGame::Ref<GraphicCore::CoreImage> KtxTexture2DArray::LoadTexture (const std::string& filePatch,
                                                                        const vk::Format&  format)
{
    KtxTextureLoader            TextureLoader { };
    GraphicCore::ImageContainer imgLoadedData = TextureLoader.LoadFile (filePatch);

    GraphicCore::ImageManager imageManager {context};

    GraphicCore::CoreBufferManager bufferManager (context);
    GraphicCore::CoreBuffer        staging;
    bufferManager.createStagingBuffer (staging, imgLoadedData.TextureSize, imgLoadedData.TextureData);

    // Setup buffer copy regions for each layer including all of it's miplevels
    std::vector<vk::BufferImageCopy> bufferCopyRegions;

    for (uint32_t layer = 0; layer < imgLoadedData.arrayLayer; ++layer)
    {
        for (uint32_t level = 0; level < imgLoadedData.mipLevels; ++level)
        {
            ktx_size_t     offset {0};
            KTX_error_code result =
                ktxTexture_GetImageOffset (TextureLoader.GetKtxTexture( ), level, layer, 0, &offset);
            assert (result == KTX_SUCCESS);

            vk::BufferImageCopy bufferCopyRegion             = { };
            bufferCopyRegion.imageSubresource.aspectMask     = vk::ImageAspectFlagBits::eColor;
            bufferCopyRegion.imageSubresource.mipLevel       = level;
            bufferCopyRegion.imageSubresource.baseArrayLayer = layer;
            bufferCopyRegion.imageSubresource.layerCount     = 1;
            bufferCopyRegion.imageExtent.width               = imgLoadedData.TextureExtend.width >> level;
            bufferCopyRegion.imageExtent.height              = imgLoadedData.TextureExtend.height >> level;
            bufferCopyRegion.imageExtent.depth               = 1;
            bufferCopyRegion.bufferOffset                    = offset;

            bufferCopyRegions.push_back (bufferCopyRegion);
        }
    }
    // check ref count of result
    auto result = imageManager.CreateImage (imgLoadedData, format, vk::ImageTiling::eOptimal,
                                            vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eTransferDst,
                                            vk::MemoryPropertyFlagBits::eDeviceLocal, vk::SampleCountFlagBits::e1);

    VulkanGame::Ref<GraphicCore::CoreImageView> view =
        VulkanGame::CreateRef<GraphicCore::CoreImageView> (context.getVkDevice( ));
    view->CreateImageView (result, vk::ImageViewType::e2DArray);

    result->GetViews( ).insert (view);

    // maybe use imageManager.transitionImageLayout

    GraphicCore::SingleTimeCommandBuffer singleTimeCmdBuffer (context);
    singleTimeCmdBuffer.createCommandBuffer (true);

    // Image barrier for optimal image (target)
    // Set initial layout for all array layers (faces) of the optimal (target) tiled texture
    vk::ImageSubresourceRange subresourceRange = view->getSubresourceRange( );

    GraphicCore::vkHelper::setImageLayout (singleTimeCmdBuffer.getCommandBuffer( ), result->GetVkImage( ),
                                           vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal,
                                           subresourceRange);

    // Copy the layers and mip levels from the staging buffer to the optimal tiled image
    singleTimeCmdBuffer.getCommandBuffer( ).copyBufferToImage (staging.getBuffer( ), result->GetVkImage( ),
                                                               vk::ImageLayout::eTransferDstOptimal, bufferCopyRegions);

    // Change texture image layout to shader read after all faces have been copied
    GraphicCore::vkHelper::setImageLayout (singleTimeCmdBuffer.getCommandBuffer( ), result->GetVkImage( ),
                                           vk::ImageLayout::eTransferDstOptimal,
                                           vk::ImageLayout::eShaderReadOnlyOptimal, subresourceRange);

    singleTimeCmdBuffer.endSingleTimeCommands (true);

    // imageManager.createImageView (result, vk::ImageAspectFlagBits::eColor, vk::ImageViewType::e2DArray);

    return result;
}
