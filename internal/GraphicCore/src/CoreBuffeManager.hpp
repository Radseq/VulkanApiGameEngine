#ifndef CORE_BUFFER_MANAGER_HPP
#define CORE_BUFFER_MANAGER_HPP

#include <SingleTimeCommandBuffer.hpp>

#include "CoreBuffer.hpp"
#include "VulkanDevice.hpp"

namespace GraphicCore
{
    class VulkanDevice;

    class CoreBufferManager
    {
        const VulkanDevice& device;

       public:
        CoreBufferManager (const VulkanDevice& Device);

        // Template specialization for texture objects
        template <typename T> void createStagingBuffer (GraphicCore::CoreBuffer& result, const T& data) const
        {
            return createStagingBuffer (result, sizeof (T), &data);
        }

        template <typename T>
        void createStagingBuffer (GraphicCore::CoreBuffer& result, const std::vector<T>& data) const
        {
            return createStagingBuffer (result, data.size( ) * sizeof (T), (void*) data.data( ));
        }

        template <typename T>
        void stageToDeviceBuffer (GraphicCore::CoreBuffer& result, const vk::BufferUsageFlags& usage,
                                  const std::vector<T>& data) const
        {
            return stageToDeviceBuffer (result, usage, sizeof (T) * data.size( ), data.data( ));
        }

        template <typename T>
        void stageToDeviceBuffer (GraphicCore::CoreBuffer& result, const vk::BufferUsageFlags& usage,
                                  const T& data) const
        {
            stageToDeviceBuffer (result, usage, sizeof (T), (void*) &data);
        }

        template <typename T> void createUniformBuffer (GraphicCore::CoreBuffer& result, const T& data) const
        {
            createSizedUniformBuffer (result, sizeof (T));
            result.map( );
            result.copy (data);
        }

        void stageToDeviceBuffer (GraphicCore::CoreBuffer& result, const vk::BufferUsageFlags& usage, size_t size,
                                  const void* data) const;

        void createBuffer (GraphicCore::CoreBuffer& result, const vk::BufferUsageFlags& usageFlags,
                           const vk::MemoryPropertyFlags& memoryPropertyFlags, const vk::DeviceSize& size) const;

        void createStagingBuffer (GraphicCore::CoreBuffer& result, const vk::DeviceSize& size,
                                  const void* data = nullptr) const;

        void createDeviceBuffer (GraphicCore::CoreBuffer& result, const vk::BufferUsageFlags& usageFlags,
                                 const vk::DeviceSize& size) const;

        void createSizedUniformBuffer (GraphicCore::CoreBuffer& result, const vk::DeviceSize& size) const;
    };
}  // namespace GraphicCore

#endif CORE_BUFFER_MANAGER_HPP
