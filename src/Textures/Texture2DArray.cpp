#include "Texture2DArray.hpp"

void Texture2DArray::allocateImgMemory (GameCore::Image& imageResult, const vk::MemoryPropertyFlags& properties) const
{
    const vk::MemoryRequirements memReqs = context.getVkDevice( ).getImageMemoryRequirements (imageResult.image);

    GameCore::AllocationCreateInfo allocInfo { };
    allocInfo.size            = memReqs.size;
    allocInfo.memoryTypeIndex = context.getMemoryType (memReqs.memoryTypeBits, properties);
    allocInfo.usage           = properties;

    context.getAllocator( )->alloc (imageResult.allocatedMemory, allocInfo);
    imageResult.bind( );
}

Texture2DArray::Texture2DArray (const GameCore::VulkanDevice& Context)
    : context (Context)
{
}

void Texture2DArray::LoadTexture (GameCore::Image& result, const std::string& filePatch, const vk::Format& format)
{
    result.device = context.getVkDevice( );
    result.format = vk::Format::eBc3UnormBlock;

    ktxResult   loadresult = KTX_SUCCESS;
    ktxTexture* texture;

    loadresult = ktxTexture_CreateFromNamedFile (filePatch.c_str( ), KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &texture);

    if (loadresult != KTX_SUCCESS) throw std::runtime_error ("\n\nNot Found. patchFile: " + filePatch);

    assert (texture && "file loaded but data is null");

    vk::Extent3D textureSize {texture->baseWidth, texture->baseHeight, 1};

    result.mipLevels  = texture->numLevels;
    result.extent     = textureSize;
    result.layerCount = texture->numLayers;

    auto textData = ktxTexture_GetData (texture);
    auto textSize = ktxTexture_GetSize (texture);

    GameCore::ImageManager imageManager {context};

    GameCore::CoreBufferManager bufferManager (context);
    GameCore::CoreBuffer        staging;
    bufferManager.createStagingBuffer (staging, textSize, textData);

    // Setup buffer copy regions for each layer including all of it's miplevels
    std::vector<vk::BufferImageCopy> bufferCopyRegions;

    for (uint32_t layer = 0; layer < result.layerCount; ++layer)
    {
        for (uint32_t level = 0; level < result.mipLevels; ++level)
        {
            ktx_size_t     offset;
            KTX_error_code result = ktxTexture_GetImageOffset (texture, level, layer, 0, &offset);
            assert (result == KTX_SUCCESS);

            vk::BufferImageCopy bufferCopyRegion             = { };
            bufferCopyRegion.imageSubresource.aspectMask     = vk::ImageAspectFlagBits::eColor;
            bufferCopyRegion.imageSubresource.mipLevel       = level;
            bufferCopyRegion.imageSubresource.baseArrayLayer = layer;
            bufferCopyRegion.imageSubresource.layerCount     = 1;
            bufferCopyRegion.imageExtent.width               = texture->baseWidth >> level;
            bufferCopyRegion.imageExtent.height              = texture->baseHeight >> level;
            bufferCopyRegion.imageExtent.depth               = 1;
            bufferCopyRegion.bufferOffset                    = offset;

            bufferCopyRegions.push_back (bufferCopyRegion);
        }
    }

    // Create optimal tiled target image
    vk::ImageCreateInfo imageCreateInfo;
    imageCreateInfo.imageType     = vk::ImageType::e2D;
    imageCreateInfo.format        = format;
    imageCreateInfo.samples       = vk::SampleCountFlagBits::e1;
    imageCreateInfo.tiling        = vk::ImageTiling::eOptimal;
    imageCreateInfo.sharingMode   = vk::SharingMode::eExclusive;
    imageCreateInfo.initialLayout = vk::ImageLayout::eUndefined;
    imageCreateInfo.extent        = textureSize;
    imageCreateInfo.usage         = vk::ImageUsageFlagBits::eSampled;
    // Ensure that the TRANSFER_DST bit is set for staging
    if (!(imageCreateInfo.usage & vk::ImageUsageFlagBits::eTransferDst))
    { imageCreateInfo.usage |= vk::ImageUsageFlagBits::eTransferDst; }
    imageCreateInfo.arrayLayers = result.layerCount;
    imageCreateInfo.mipLevels   = result.mipLevels;

    result.image = context.getVkDevice( ).createImage (imageCreateInfo);

    allocateImgMemory (result, vk::MemoryPropertyFlagBits::eDeviceLocal);

    // maybe use imageManager.transitionImageLayout
    
    GameCore::SingleTimeCommandBuffer singleTimeCmdBuffer (context);
    singleTimeCmdBuffer.createCommandBuffer (true);

    // Image barrier for optimal image (target)
    // Set initial layout for all array layers (faces) of the optimal (target) tiled texture
    vk::ImageSubresourceRange subresourceRange = { };
    subresourceRange.aspectMask                = vk::ImageAspectFlagBits::eColor;
    subresourceRange.baseMipLevel              = 0;
    subresourceRange.levelCount                = result.mipLevels;
    subresourceRange.layerCount                = result.layerCount;

    GameCore::vkHelper::setImageLayout (singleTimeCmdBuffer.getCommandBuffer( ), result.image,
                                        vk::ImageLayout::eUndefined,
                                        vk::ImageLayout::eTransferDstOptimal, subresourceRange);

    // Copy the layers and mip levels from the staging buffer to the optimal tiled image
    singleTimeCmdBuffer.getCommandBuffer( ).copyBufferToImage (staging.getBuffer( ), result.image,
                                                               vk::ImageLayout::eTransferDstOptimal, bufferCopyRegions);

    // Change texture image layout to shader read after all faces have been copied
    GameCore::vkHelper::setImageLayout (singleTimeCmdBuffer.getCommandBuffer( ), result.image,
                                        vk::ImageLayout::eTransferDstOptimal,
                                        vk::ImageLayout::eShaderReadOnlyOptimal, subresourceRange);

    singleTimeCmdBuffer.endSingleTimeCommands (true);

    imageManager.createImageView (result, vk::ImageAspectFlagBits::eColor, vk::ImageViewType::e2DArray);
}
