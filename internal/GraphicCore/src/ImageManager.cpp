#include "pch.h"
#include "ImageManager.hpp"

namespace GraphicCore
{
    void ImageManager::allocateImgMemory (std::shared_ptr<GraphicCore::CoreImage> imageResult,
                                          const vk::MemoryPropertyFlags&          properties) const
    {
        const vk::MemoryRequirements memReqs =
            device.getVkDevice( ).getImageMemoryRequirements (imageResult->GetVkImage( ));
        /*vk::MemoryAllocateInfo memAllocInfo;
        memAllocInfo.allocationSize = result.getAllocSize() = memReqs.size;
        memAllocInfo.memoryTypeIndex = device.getMemoryType(memReqs.memoryTypeBits, memoryPropertyFlags);
        result.getMemory() = device.getVkDevice().allocateMemory(memAllocInfo);
        device.getVkDevice().bindImageMemory(result.getVkImage(), result.getMemory(), 0);*/

        AllocationCreateInfo allocInfo { };
        allocInfo.size            = memReqs.size;
        allocInfo.memoryTypeIndex = device.getMemoryType (memReqs.memoryTypeBits, properties);
        allocInfo.usage           = properties;

        device.getAllocator( )->alloc (imageResult->allocatedMemory, allocInfo);
        imageResult->bind( );
    }

    ImageManager::ImageManager (const VulkanDevice& Device)
        : device (Device)
    {
    }

    std::shared_ptr<GraphicCore::CoreImage> ImageManager::CreateImage (const ImageContainer&          imgContainer,
                                                                       const vk::Format&              format,
                                                                       const vk::ImageTiling&         tiling,
                                                                       const vk::ImageUsageFlags&     usage,
                                                                       const vk::MemoryPropertyFlags& properties,
                                                                       const vk::SampleCountFlagBits& numSamples)
    {
        std::shared_ptr<GraphicCore::CoreImage> result = std::make_shared<GraphicCore::CoreImage> (
            device.getVkDevice( ), imgContainer.TextureExtend, format, usage, properties, numSamples,
            imgContainer.mipLevels, imgContainer.arrayLayer, tiling);

        vk::ImageCreateInfo imageInfo = { };
        imageInfo.imageType           = vk::ImageType::e2D;
        imageInfo.extent              = imgContainer.TextureExtend;
        imageInfo.mipLevels           = imgContainer.mipLevels;
        imageInfo.arrayLayers         = imgContainer.arrayLayer;
        imageInfo.format              = format;
        imageInfo.tiling              = tiling;
        imageInfo.initialLayout       = vk::ImageLayout::eUndefined;
        imageInfo.usage               = usage;
        imageInfo.samples             = numSamples;
        imageInfo.sharingMode         = vk::SharingMode::eExclusive;

        result->m_Image = device.getVkDevice( ).createImage (imageInfo);

        allocateImgMemory (result, properties);
        return result;
    }

    void ImageManager::generateMipmaps (const GraphicCore::CoreImage& coreImage)
    {
        vk::FormatProperties formatProperties =
            device.getDevice( ).GetPhysicalDevice( ).getFormatProperties (coreImage.GetFormat( ));

        // Check if image format supports linear blitting
        if (!(formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eSampledImageFilterLinear))
        {
            throw std::runtime_error ("texture image format does not support linear blitting!");
        }

        SingleTimeCommandBuffer singleTimeCmdBuffer (device);
        singleTimeCmdBuffer.createCommandBuffer (true);

        vk::ImageMemoryBarrier barrier          = { };
        barrier.image                           = coreImage.m_Image;
        barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
        barrier.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount     = 1;
        barrier.subresourceRange.levelCount     = 1;

        const vk::Extent3D& imgExtend = coreImage.GetExtend( );

        vk::Offset3D mipSize {GraphicCore::Util::toInt32 (imgExtend.width),
                              GraphicCore::Util::toInt32 (imgExtend.height), 1};

        vk::Offset3D emptyOffset {0, 0, 0};

        for (uint32_t i = 1; i < coreImage.GetMipLevels( ); i++)
        {
            barrier.subresourceRange.baseMipLevel = i - 1;
            barrier.oldLayout                     = vk::ImageLayout::eTransferDstOptimal;
            barrier.newLayout                     = vk::ImageLayout::eTransferSrcOptimal;
            barrier.srcAccessMask                 = vk::AccessFlagBits::eTransferWrite;
            barrier.dstAccessMask                 = vk::AccessFlagBits::eTransferRead;

            singleTimeCmdBuffer.getCommandBuffer( ).pipelineBarrier (vk::PipelineStageFlagBits::eTransfer,
                                                                     vk::PipelineStageFlagBits::eTransfer, { }, nullptr,
                                                                     nullptr, barrier);

            vk::ImageBlit blit                 = { };
            auto          scrOffset            = std::array<vk::Offset3D, 2> {emptyOffset, mipSize};
            blit.srcOffsets                    = scrOffset;
            blit.srcSubresource.aspectMask     = vk::ImageAspectFlagBits::eColor;
            blit.srcSubresource.mipLevel       = i - 1;
            blit.srcSubresource.baseArrayLayer = 0;
            blit.srcSubresource.layerCount     = coreImage.GetLayerCount( );
            vk::Offset3D destOffset {mipSize.x > 1 ? mipSize.x / 2 : 1, mipSize.y > 1 ? mipSize.y / 2 : 1, 1};
            auto         dstOffsets            = std::array<vk::Offset3D, 2> {emptyOffset, destOffset};
            blit.dstOffsets                    = dstOffsets;
            blit.dstSubresource.aspectMask     = vk::ImageAspectFlagBits::eColor;
            blit.dstSubresource.mipLevel       = i;
            blit.dstSubresource.baseArrayLayer = 0;
            blit.dstSubresource.layerCount     = 1;

            singleTimeCmdBuffer.getCommandBuffer( ).blitImage (
                coreImage.GetVkImage( ), vk::ImageLayout::eTransferSrcOptimal, coreImage.GetVkImage( ),
                vk::ImageLayout::eTransferDstOptimal, blit, vk::Filter::eLinear);

            barrier.oldLayout     = vk::ImageLayout::eTransferSrcOptimal;
            barrier.newLayout     = vk::ImageLayout::eShaderReadOnlyOptimal;
            barrier.srcAccessMask = vk::AccessFlagBits::eTransferRead;
            barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

            singleTimeCmdBuffer.getCommandBuffer( ).pipelineBarrier (vk::PipelineStageFlagBits::eTransfer,
                                                                     vk::PipelineStageFlagBits::eFragmentShader, { },
                                                                     nullptr, nullptr, barrier);

            mipSize = blit.dstOffsets [1];

            // if (mipSize.x > 1) mipSize.x /= 2;
            // if (mipSize.y > 1) mipSize.y /= 2;
        }

        barrier.subresourceRange.baseMipLevel = coreImage.GetMipLevels( ) - 1;
        barrier.oldLayout                     = vk::ImageLayout::eTransferDstOptimal;
        barrier.newLayout                     = vk::ImageLayout::eShaderReadOnlyOptimal;
        barrier.srcAccessMask                 = vk::AccessFlagBits::eTransferWrite;
        barrier.dstAccessMask                 = vk::AccessFlagBits::eShaderRead;

        singleTimeCmdBuffer.getCommandBuffer( ).pipelineBarrier (vk::PipelineStageFlagBits::eTransfer,
                                                                 vk::PipelineStageFlagBits::eFragmentShader, { },
                                                                 nullptr, nullptr, barrier);

        singleTimeCmdBuffer.endSingleTimeCommands (true);
    }

    void ImageManager::transitionImageLayout (const GraphicCore::CoreImage&               coreImage,
                                              const GraphicCore::SingleTimeCommandBuffer& cmdBuff,
                                              const vk::ImageLayout& oldLayout, const vk::ImageLayout& newLayout)
    {
        vk::ImageMemoryBarrier barrier          = { };
        barrier.oldLayout                       = oldLayout;
        barrier.newLayout                       = newLayout;
        barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
        barrier.image                           = coreImage.GetVkImage( );
        barrier.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
        barrier.subresourceRange.baseMipLevel   = 0;
        barrier.subresourceRange.levelCount     = coreImage.GetMipLevels( );
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount     = coreImage.GetLayerCount( );

        vk::PipelineStageFlags sourceStage;
        vk::PipelineStageFlags destinationStage;

        if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal)
        {
            barrier.srcAccessMask = vk::AccessFlagBits::eIndirectCommandRead;  //??????? should be 0 not 01
            barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

            sourceStage      = vk::PipelineStageFlagBits::eTopOfPipe;
            destinationStage = vk::PipelineStageFlagBits::eTransfer;
        }
        else if (oldLayout == vk::ImageLayout::eTransferDstOptimal &&
                 newLayout == vk::ImageLayout::eShaderReadOnlyOptimal)
        {
            barrier.srcAccessMask = vk::AccessFlagBits::eShaderRead;
            barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

            sourceStage      = vk::PipelineStageFlagBits::eTransfer;
            destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
        }
        else
        {
            throw std::invalid_argument ("unsupported layout transition!");
        }
    }

    /*void ImageManager::createSampler (std::shared_ptr<GraphicCore::CoreImage> coreImage)
    {
        vk::Bool32 samplerAnisotropy = device.getDevice( ).GetPhysicalDeviceFeatures( ).samplerAnisotropy;

        vk::SamplerCreateInfo samplerInfo = { };
        samplerInfo.magFilter             = vk::Filter::eLinear;
        samplerInfo.minFilter             = vk::Filter::eLinear;
        samplerInfo.addressModeU          = vk::SamplerAddressMode::eRepeat;
        samplerInfo.addressModeV          = vk::SamplerAddressMode::eRepeat;
        samplerInfo.addressModeW          = vk::SamplerAddressMode::eRepeat;
        samplerInfo.anisotropyEnable      = samplerAnisotropy;
        samplerInfo.maxAnisotropy =
            samplerAnisotropy ? device.getDevice( ).GetDeviceProperties( ).limits.maxSamplerAnisotropy : 1.0F;
        samplerInfo.borderColor             = vk::BorderColor::eFloatOpaqueWhite;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable           = VK_FALSE;
        samplerInfo.compareOp               = vk::CompareOp::eNever;
        samplerInfo.mipmapMode              = vk::SamplerMipmapMode::eLinear;
        samplerInfo.minLod                  = 0;
        samplerInfo.maxLod                  = static_cast<float> (coreImage->GetMipLevels( ));
        samplerInfo.mipLodBias              = 0;

        coreImage->sampler = device.getVkDevice( ).createSampler (samplerInfo);
    }*/

    void ImageManager::copyBufferToImage (const vk::Buffer& buffer, std::shared_ptr<GraphicCore::CoreImage> coreImage)
    {
        GraphicCore::SingleTimeCommandBuffer singleTimeCmdBuffer (device);
        singleTimeCmdBuffer.createCommandBuffer (true);

        const vk::Extent3D& imgExtend = coreImage->GetExtend( );
        vk::Extent2D        imageSize (imgExtend.width, imgExtend.height);

        // Setup buffer copy regions for each mip level
        std::vector<vk::BufferImageCopy> bufferCopyRegions;

        vk::Offset3D emptyOffset {0, 0, 0};

        for (uint32_t i = 0; i < coreImage->GetMipLevels( ); i++)
        {
            vk::Extent3D imageExtent {imageSize.width >> i, imageSize.height >> i, 1};

            vk::BufferImageCopy region = { };
            region.bufferOffset        = 0;
            // region.bufferRowLength = 0;
            // region.bufferImageHeight = 0;
            region.imageSubresource.aspectMask     = vk::ImageAspectFlagBits::eColor;
            region.imageSubresource.mipLevel       = i;
            region.imageSubresource.baseArrayLayer = 0;
            region.imageSubresource.layerCount     = 1;
            region.imageOffset                     = emptyOffset;
            region.imageExtent                     = imageExtent;

            bufferCopyRegions.push_back (region);
        }

        vk::ImageSubresourceRange range;

        for (auto& view : coreImage->GetViews( )) { range = view->getSubresourceRange( ); }

        // Prepare for transfer
        vkHelper::setImageLayout (singleTimeCmdBuffer.getCommandBuffer( ), coreImage->GetVkImage( ),
                                  vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal, range);

        singleTimeCmdBuffer.getCommandBuffer( ).copyBufferToImage (
            buffer, coreImage->GetVkImage( ), vk::ImageLayout::eTransferDstOptimal, bufferCopyRegions);

        vkHelper::setImageLayout (singleTimeCmdBuffer.getCommandBuffer( ), coreImage->GetVkImage( ),
                                  vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal, range);

        singleTimeCmdBuffer.endSingleTimeCommands (true);
    }

    /*
    void ImageManager::createImageView (std::shared_ptr<GraphicCore::CoreImage> coreImage,
                                        const vk::ImageAspectFlags& aspectFlags, const vk::ImageViewType& imgViewType)
    {
        vk::ImageViewCreateInfo viewInfo         = { };
        viewInfo.image                           = coreImage->GetVkImage( );
        viewInfo.viewType                        = imgViewType;
        viewInfo.format                          = coreImage->GetFormat( );
        viewInfo.subresourceRange.aspectMask     = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel   = 0;
        viewInfo.subresourceRange.levelCount     = coreImage->GetMipLevels( );
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount     = coreImage->GetLayerCount( );

        viewInfo.components = {vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB,
                               vk::ComponentSwizzle::eA};

        coreImage->view = device.getVkDevice( ).createImageView (viewInfo);
    }*/
    /*
    std::shared_ptr<GraphicCore::CoreImage> ImageManager::stageToDeviceImage (
        vk::ImageCreateInfo&           imageCreateInfo,  // should be there reference?
        const vk::MemoryPropertyFlags& memoryPropertyFlags, const vk::DeviceSize& size, const void* data,
        const std::vector<MipData>& mipData, const vk::ImageLayout layout) const
    {
        CoreBufferManager       bufferManager (device);
        GraphicCore::CoreBuffer staging;
        bufferManager.createStagingBuffer (staging, size, data);
        imageCreateInfo.usage = imageCreateInfo.usage | vk::ImageUsageFlagBits::eTransferDst;

        auto stagResult = CreateImage (imageCreateInfo, memoryPropertyFlags);

        GraphicCore::SingleTimeCommandBuffer singleTimeCmdBuffer (device);
        singleTimeCmdBuffer.createCommandBuffer (true);

        const vk::ImageSubresourceRange range (vk::ImageAspectFlagBits::eColor, 0, imageCreateInfo.mipLevels, 0, 1);
        // Prepare for transfer
        vkHelper::setImageLayout (singleTimeCmdBuffer.getCommandBuffer( ), stagResult->GetVkImage( ),
                                  vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal, range);

        // Prepare for transfer
        std::vector<vk::BufferImageCopy> bufferCopyRegions;
        {
            vk::BufferImageCopy bufferCopyRegion;
            bufferCopyRegion.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
            bufferCopyRegion.imageSubresource.layerCount = 1;
            if (!mipData.empty( ))
            {
                for (uint32_t i = 0; i < imageCreateInfo.mipLevels; i++)
                {
                    bufferCopyRegion.imageSubresource.mipLevel = i;
                    bufferCopyRegion.imageExtent               = mipData [i].first;
                    bufferCopyRegions.push_back (bufferCopyRegion);
                    bufferCopyRegion.bufferOffset += mipData [i].second;
                }
            }
            else
            {
                bufferCopyRegion.imageExtent = imageCreateInfo.extent;
                bufferCopyRegions.push_back (bufferCopyRegion);
            }
        }
        singleTimeCmdBuffer.getCommandBuffer( ).copyBufferToImage (
            staging.getBuffer( ), stagResult->GetVkImage( ), vk::ImageLayout::eTransferDstOptimal, bufferCopyRegions);
        // Prepare for shader read
        vkHelper::setImageLayout (singleTimeCmdBuffer.getCommandBuffer( ), stagResult->GetVkImage( ),
                                  vk::ImageLayout::eTransferDstOptimal, layout, range);

        singleTimeCmdBuffer.endSingleTimeCommands (true);

        staging.destroy (device);

        return stagResult;
    }*/
}  // namespace GraphicCore
