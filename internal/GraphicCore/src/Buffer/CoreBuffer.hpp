#ifndef CORE_BUFFER_HPP
#define CORE_BUFFER_HPP

#include <Device\VulkanDevice.hpp>
#include <vulkan/vulkan.hpp>

#include "Allocation/AllocationProperties.hpp"

namespace GraphicCore
{
    class CoreBuffer
    {
        vk::Device               device;
        vk::Buffer               buffer;
        vk::DescriptorBufferInfo descriptor;

        AllocationProperties allocatedMemory { };
        void*                mapped {nullptr};

       public:
        friend class CoreBufferManager;

        const vk::Buffer&               getBuffer( ) const;
        const vk::Device&               getDevice( ) const;
        const vk::DescriptorBufferInfo& getDescriptorInfo( ) const;
        const AllocationProperties&     getAllocatedMemory( );

        explicit operator bool( ) const { return buffer.operator bool( ); }

        void bind( ) const;
        void unMap( );
        void copy (const size_t& size, const void* data, const vk::DeviceSize& offset);

        template <typename T = void> T* map (vk::DeviceSize size = VK_WHOLE_SIZE)
        {
            mapped = device.mapMemory (allocatedMemory.handle, allocatedMemory.offset, size, vk::MemoryMapFlags( ));
            return (T*) mapped;
        }

        template <typename T> void copy (const T& data, vk::DeviceSize offset = 0) { copy (sizeof (T), &data, offset); }

        template <typename T> void copy (const std::vector<T>& data, vk::DeviceSize offset = 0)
        {
            copy (sizeof (T) * data.size( ), data.data( ), offset);
        }
        /*
        template <typename T> void copyToMemory (const T& data, size_t offset = 0) const {
            vk::DeviceSize size {sizeof (T)};

            void* mapped =
                device.mapMemory (allocatedMemory.handle, allocatedMemory.offset, size, vk::MemoryMapFlags( ));
            memcpy (mapped, &data, static_cast<size_t> (size));
            device.unmapMemory (allocatedMemory.handle);
        }*/

        /**
         * Setup the default descriptor for this buffer
         *
         * @param size (Optional) Size of the memory range of the descriptor
         * @param offset (Optional) Byte offset from beginning
         *
         */
        void setupDescriptor (const vk::DeviceSize& size = VK_WHOLE_SIZE, const vk::DeviceSize& offset = 0);

        /**
         * Release all Vulkan resources held by this buffer
         */
        void destroy (const GraphicCore::VulkanDevice& context);
    };
}  // namespace GraphicCore

#endif CORE_BUFFER_HPP
