#include "pch.h"
#include "CoreBuffer.hpp"

namespace GraphicCore {
    /// <summary>
    /// Attach the allocated memory block to the buffer
    /// </summary>
    void CoreBuffer::bind( ) const {
        device.bindBufferMemory (buffer, allocatedMemory.handle, allocatedMemory.offset);
    }

    /**
     * Setup the default descriptor for this buffer
     *
     * @param size (Optional) Size of the memory range of the descriptor
     * @param offset (Optional) Byte offset from beginning
     *
     */
    void CoreBuffer::setupDescriptor (const vk::DeviceSize& size, const vk::DeviceSize& offset) {
        descriptor.offset = offset;
        descriptor.buffer = buffer;
        descriptor.range  = size;
    }

    void CoreBuffer::copy (const size_t& size, const void* data, const vk::DeviceSize& offset) {
        memcpy (static_cast<uint8_t*> (mapped) + offset, data, size);
    };

    /**
     * Release all Vulkan resources held by this buffer
     */
    void CoreBuffer::destroy (const GraphicCore::VulkanDevice& context) {
        if (buffer) {
            device.destroy (buffer);
            buffer = vk::Buffer { };
        }

        //context.getAllocator( )->free (allocatedMemory);
    }

    void CoreBuffer::unMap( ) { device.unmapMemory (allocatedMemory.handle); }

    const vk::Buffer&               CoreBuffer::getBuffer( ) const { return buffer; }
    const vk::Device&               CoreBuffer::getDevice( ) const { return device; }
    const vk::DescriptorBufferInfo& CoreBuffer::getDescriptorInfo( ) const { return descriptor; }
    const AllocationProperties&     CoreBuffer::getAllocatedMemory( ) { return allocatedMemory; }
}  // namespace GraphicCore
