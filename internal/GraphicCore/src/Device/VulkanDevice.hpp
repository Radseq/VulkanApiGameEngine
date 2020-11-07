#ifndef VULKAN_DEVICE_HPP
#define VULKAN_DEVICE_HPP

#include <queue>

#include "Allocation/AllocationPool.hpp"
#include "Allocation/IAllocation.hpp"
#include "PhysicalDevice.hpp"

namespace GraphicCore
{
    class PhysicalDevice;

    class VulkanDevice
    {
#ifdef WIN32
        static __declspec(thread) vk::CommandPool s_cmdPool;
#else
        static thread_local vk::CommandPool s_cmdPool;
#endif

        PhysicalDevice& physicalDevice;  // physical device (GPU)
        vk::Device      device;          // Logical device, application's view of the

        vk::Queue graphicsQueue;
        vk::Queue presentQueue;

        vk::Format getSupportedDepthFormat( ) const;
        vk::Format findSupportedFormat (const std::vector<vk::Format>& candidates, vk::ImageTiling tiling,
                                        const vk::FormatFeatureFlags& features) const;

        std::unique_ptr<IAllocation> allocator;

       public:
        VulkanDevice (PhysicalDevice& PhysicalDevice);

        void Destroy( ) const;
        void CreateDevice( );
        void CreateCommandPool( ) const;

        const std::unique_ptr<IAllocation>& getAllocator( ) const;

        const vk::Device&     getVkDevice( ) const;
        const PhysicalDevice& getDevice( ) const;
        const vk::Queue&      getGraphicsQueue( ) const;
        const vk::Queue&      getPresentQueue( ) const;

        const vk::CommandPool& getCommandPool( ) const;

        template <typename T>
        void copyToMemory (const vk::DeviceMemory& memory, const T& data, const vk::DeviceSize& size,
                           size_t offset = 0) const
        {
            void* mapped = device.mapMemory (memory, offset, size, vk::MemoryMapFlags( ));
            memcpy (mapped, data, static_cast<size_t> (size));
            device.unmapMemory (memory);
        }
        /*
        void copyToMemory(const vk::DeviceMemory& memory, const void* data,
        vk::DeviceSize size, vk::DeviceSize offset = 0) const;

        template <typename T>
        void copyToMemory(const vk::DeviceMemory& memory, const T& data, size_t
        offset = 0) const
        {
                copyToMemory(memory, &data, sizeof(T), offset);
        }

        template <typename T>
        void copyToMemory(const vk::DeviceMemory& memory, const std::vector<T>&
        data, size_t offset = 0) const
        {
                copyToMemory(memory, data.data(), data.size() * sizeof(T),
        offset);
        }*/

        uint32_t getMemoryType (const uint32_t& typeBits, const vk::MemoryPropertyFlags& properties) const;

        // Find a suitable depth format
        vk::Format depthFormat = getSupportedDepthFormat( );
    };
}  // namespace GraphicCore

#endif  // VULKAN_DEVICE_HPP
