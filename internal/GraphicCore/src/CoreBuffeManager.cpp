#include "pch.h"
#include "CoreBuffeManager.hpp"

namespace GraphicCore
{
    CoreBufferManager::CoreBufferManager (const VulkanDevice& Device)
        : device (Device)
    {
    }

    void CoreBufferManager::stageToDeviceBuffer (GraphicCore::CoreBuffer& result, const vk::BufferUsageFlags& usage,
                                                 size_t size, const void* data) const
    {
        GraphicCore::CoreBuffer staging;

        createStagingBuffer (staging, size, data);
        createDeviceBuffer (result, usage | vk::BufferUsageFlagBits::eTransferDst, size);

        GraphicCore::SingleTimeCommandBuffer singleTimeCmdBuffer (device);
        singleTimeCmdBuffer.createCommandBuffer (true);

        vk::BufferCopy copyRegion = {0, 0, size};

        singleTimeCmdBuffer.getCommandBuffer( ).copyBuffer (staging.getBuffer( ), result.getBuffer( ), copyRegion);
        singleTimeCmdBuffer.endSingleTimeCommands (true);

        staging.destroy (device);
    }

    void CoreBufferManager::createBuffer (GraphicCore::CoreBuffer& result, const vk::BufferUsageFlags& usageFlags,
                                          const vk::MemoryPropertyFlags& memoryPropertyFlags,
                                          const vk::DeviceSize&          size) const
    {
        result.device            = device.getVkDevice( );
        result.descriptor.range  = VK_WHOLE_SIZE;
        result.descriptor.offset = 0;

        vk::BufferCreateInfo bufferCreateInfo { };
        bufferCreateInfo.usage = usageFlags;
        bufferCreateInfo.size  = size;
        bufferCreateInfo.setSharingMode (vk::SharingMode::eExclusive);

        result.descriptor.buffer = result.buffer = device.getVkDevice( ).createBuffer (bufferCreateInfo);

        // Prepare and initialize a uniform buffer block containing shader uniforms
        // In Vulkan there are no more single uniforms like in GL
        // All shader uniforms are passed as uniform buffer blocks
        // Get memory requirements including size, alignment and memory type
        const vk::MemoryRequirements memReqs = device.getVkDevice( ).getBufferMemoryRequirements (result.getBuffer( ));
        /*vk::MemoryAllocateInfo memAlloc;
        result->getAllocSize() = memAlloc.allocationSize = memReqs.size;

        // Get the memory type index that supports host visible memory access
        // Most implementations offer multiple memory types and selecting the
        // correct one to allocate memory from is important
        memAlloc.memoryTypeIndex = device.getMemoryType(memReqs.memoryTypeBits, memoryPropertyFlags);
        // Allocate memory for the uniform buffer
        result->getMemory() = device.getVkDevice().allocateMemory(memAlloc);
        // Bind memory to buffer
        device.getVkDevice().bindBufferMemory(result->getBuffer(), result->getMemory(), 0);*/

        AllocationCreateInfo allocInfo { };
        allocInfo.size = memReqs.size;
        // Get the memory type index that supports host visible memory access
        // Most implementations offer multiple memory types and selecting the
        // correct one to allocate memory from is important
        allocInfo.memoryTypeIndex = device.getMemoryType (memReqs.memoryTypeBits, memoryPropertyFlags);
        allocInfo.usage           = memoryPropertyFlags;

        if (usageFlags & vk::BufferUsageFlagBits::eShaderDeviceAddress) { allocInfo.bufferUsageShader = true; }

        device.getAllocator( )->alloc (result.allocatedMemory, allocInfo);
        result.bind( );
    }

    // Static data like vertex and index buffer should be stored on the device memory
    // for optimal (and fastest) access by the GPU
    //
    // To achieve this we use so-called "staging buffers" :
    // - Create a buffer that's visible to the host (and can be mapped)
    // - Copy the data to this buffer
    // - Create another buffer that's local on the device (VRAM) with the same size
    // - Copy the data from the host to the device using a command buffer
    // - Delete the host visible (staging) buffer
    // - Use the device local buffers for rendering
    void CoreBufferManager::createStagingBuffer (GraphicCore::CoreBuffer& result, const vk::DeviceSize& size,
                                                 const void* data) const
    {
        createBuffer (result, vk::BufferUsageFlagBits::eTransferSrc,
                      vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, size);
        if (data != nullptr)
        {
            // device.copyToMemory (result.getAllocatedMemory( ).handle, data, size);
            result.copyToMemory (data, size);
            // ok i coped code from device.copyToMemory to GraphicCore::CoreBuffer (result)
            // and result.copyToMemory (data); dont wotk
            // device.copyToMemory (result.getAllocatedMemory( ).handle, data, size); work
            // wtf is going on....
        }
    }

    void CoreBufferManager::createDeviceBuffer (GraphicCore::CoreBuffer& result, const vk::BufferUsageFlags& usageFlags,
                                                const vk::DeviceSize& size) const
    {
        createBuffer (result, usageFlags, vk::MemoryPropertyFlagBits::eDeviceLocal, size);
    }

    void CoreBufferManager::createSizedUniformBuffer (GraphicCore::CoreBuffer& result, const vk::DeviceSize& size) const
    {
        const auto alignment     = device.getDevice( ).GetDeviceProperties( ).limits.minUniformBufferOffsetAlignment;
        const auto extra         = size % alignment;
        const auto count         = 1;
        const auto alignedSize   = size + (alignment - extra);
        const auto allocatedSize = count * alignedSize;
        createBuffer (result, vk::BufferUsageFlagBits::eUniformBuffer,
                      vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                      allocatedSize);
        result.descriptor.range = alignedSize;
    }
}  // namespace GraphicCore
