#include "Texture2DArray.hpp"

void Texture2DArray::CreateImage (GameCore::Image& texture, const ImageContainer& imgContainer) {

    GameCore::CoreBufferManager stagingBufferManager {context};
    GameCore::CoreBuffer        stagingBuffer;
    stagingBufferManager.createStagingBuffer (stagingBuffer, imgContainer.TextureSize, imgContainer.TextureData);

    GameCore::ImageManager imageManager {context};

    GameCore::ImageManager textureImageManager {context};
    textureImageManager.CreateImage (
        texture, imgContainer.TextureExtend, vk::Format::eR8G8B8A8Srgb, vk::ImageTiling::eOptimal,
        vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
        vk::MemoryPropertyFlagBits::eDeviceLocal, imgContainer.mipLevels, vk::SampleCountFlagBits::e1);

    imageManager.copyBufferToImage (stagingBuffer.getBuffer( ), texture);

    stagingBuffer.destroy (context);

    textureImageManager.generateMipmaps (texture);

    imageManager.createImageView (texture, vk::ImageAspectFlagBits::eColor);
    imageManager.createSampler (texture);
}

void Texture2DArray::allocateImgMemory (GameCore::Image& imageResult, const vk::MemoryPropertyFlags& properties) const {
    const vk::MemoryRequirements memReqs = context.getVkDevice( ).getImageMemoryRequirements (imageResult.image);
    /*vk::MemoryAllocateInfo memAllocInfo;
    memAllocInfo.allocationSize = result.getAllocSize() = memReqs.size;
    memAllocInfo.memoryTypeIndex = device.getMemoryType(memReqs.memoryTypeBits, memoryPropertyFlags);
    result.getMemory() = device.getVkDevice().allocateMemory(memAllocInfo);
    device.getVkDevice().bindImageMemory(result.getVkImage(), result.getMemory(), 0);*/

    GameCore::AllocationCreateInfo allocInfo { };
    allocInfo.size            = memReqs.size;
    allocInfo.memoryTypeIndex = context.getMemoryType (memReqs.memoryTypeBits, properties);
    allocInfo.usage           = properties;

    context.getAllocator( )->alloc (imageResult.allocatedMemory, allocInfo);
    imageResult.bind( );
}

Texture2DArray::Texture2DArray (const GameCore::VulkanDevice& Context)
    : context (Context) { }

void Texture2DArray::LoadTexture (GameCore::Image& result, const std::string& filePatch, const vk::Format& format) {
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

    GameCore::CoreBufferManager bufferManager (context);
    GameCore::CoreBuffer        staging;
    bufferManager.createStagingBuffer (staging, textSize, textData);

    // Setup buffer copy regions for each layer including all of it's miplevels
    std::vector<vk::BufferImageCopy> bufferCopyRegions;

    for (uint32_t layer = 0; layer < result.layerCount; layer++) {
        for (uint32_t level = 0; level < result.mipLevels; level++) {
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
    if (!(imageCreateInfo.usage & vk::ImageUsageFlagBits::eTransferDst)) {
        imageCreateInfo.usage |= vk::ImageUsageFlagBits::eTransferDst;
    }
    imageCreateInfo.arrayLayers = result.layerCount;
    imageCreateInfo.mipLevels   = result.mipLevels;

    result.image = context.getVkDevice( ).createImage (imageCreateInfo);

    allocateImgMemory (result, vk::MemoryPropertyFlagBits::eDeviceLocal);

    vk::CommandBufferAllocateInfo allocInfo = { };
    allocInfo.level                         = vk::CommandBufferLevel::ePrimary;
    allocInfo.commandPool                   = context.getCommandPool( );
    allocInfo.commandBufferCount            = 1;

    vk::CommandBuffer cmdBuffer = context.getVkDevice( ).allocateCommandBuffers (allocInfo) [0];

    vk::CommandBufferBeginInfo beginInfo = { };
    beginInfo.flags                      = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

    cmdBuffer.begin (beginInfo);

    // Image barrier for optimal image (target)
    // Set initial layout for all array layers (faces) of the optimal (target) tiled texture
    vk::ImageSubresourceRange subresourceRange = { };
    subresourceRange.aspectMask                = vk::ImageAspectFlagBits::eColor;
    subresourceRange.baseMipLevel              = 0;
    subresourceRange.levelCount                = result.mipLevels;
    subresourceRange.layerCount                = result.layerCount;

    GameCore::vkHelper::setImageLayout (cmdBuffer, result.image, vk::ImageLayout::eUndefined,
                                        vk::ImageLayout::eTransferDstOptimal, subresourceRange);

    // Copy the layers and mip levels from the staging buffer to the optimal tiled image

    cmdBuffer.copyBufferToImage (staging.getBuffer( ), result.image, vk::ImageLayout::eTransferDstOptimal,
                                 GameCore::util::to_uint_32_t (bufferCopyRegions.size( )), bufferCopyRegions.data( ));

    // Change texture image layout to shader read after all faces have been copied
    GameCore::vkHelper::setImageLayout (cmdBuffer, result.image, vk::ImageLayout::eTransferDstOptimal,
                                        vk::ImageLayout::eShaderReadOnlyOptimal, subresourceRange);

    cmdBuffer.end( );

    vk::SubmitInfo submitInfo     = { };
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    = &cmdBuffer;

    context.getGraphicsQueue( ).submit (submitInfo, nullptr);
    context.getGraphicsQueue( ).waitIdle( );

    context.getVkDevice( ).freeCommandBuffers (context.getCommandPool( ), cmdBuffer);

    vk::ImageViewCreateInfo viewInfo         = { };
    viewInfo.image                           = result.image;
    viewInfo.viewType                        = vk::ImageViewType::e2DArray;
    viewInfo.format                          = result.format;
    viewInfo.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
    viewInfo.subresourceRange.baseMipLevel   = 0;
    viewInfo.subresourceRange.levelCount     = result.mipLevels;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount     = result.layerCount;

    viewInfo.components = {vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB,
                           vk::ComponentSwizzle::eA};

    result.view = context.getVkDevice( ).createImageView (viewInfo);
}