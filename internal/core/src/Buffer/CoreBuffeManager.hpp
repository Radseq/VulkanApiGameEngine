#ifndef CORE_BUFFER_MANAGER_HPP
#define CORE_BUFFER_MANAGER_HPP

#include <Command\SingleTimeCommandBuffer.hpp>

#include "../Device/VulkanDevice.hpp"
#include "CoreBuffer.hpp"

namespace GameCore {
    class VulkanDevice;

    class CoreBufferManager {
        const VulkanDevice& device;

       public:
        CoreBufferManager (const VulkanDevice& Device);

        // Template specialization for texture objects
        template <typename T> void createStagingBuffer (GameCore::CoreBuffer& result, const T& data) const {
            return createStagingBuffer (result, sizeof (T), &data);
        }

        template <typename T>
        void createStagingBuffer (GameCore::CoreBuffer& result, const std::vector<T>& data) const {
            return createStagingBuffer (result, data.size( ) * sizeof (T), (void*) data.data( ));
        }

        template <typename T>
        void stageToDeviceBuffer (GameCore::CoreBuffer& result, const vk::BufferUsageFlags& usage,
                                  const std::vector<T>& data) const {
            return stageToDeviceBuffer (result, usage, sizeof (T) * data.size( ), data.data( ));
        }

        template <typename T>
        void stageToDeviceBuffer (GameCore::CoreBuffer& result, const vk::BufferUsageFlags& usage,
                                  const T& data) const {
            stageToDeviceBuffer (result, usage, sizeof (T), (void*) &data);
        }

        template <typename T> void createUniformBuffer (GameCore::CoreBuffer& result, const T& data) const {
            createSizedUniformBuffer (result, sizeof (T));
            result.map( );
            result.copy (data);
        }

        void stageToDeviceBuffer (GameCore::CoreBuffer& result, const vk::BufferUsageFlags& usage, size_t size,
                                  const void* data) const;

        void createBuffer (GameCore::CoreBuffer& result, const vk::BufferUsageFlags& usageFlags,
                           const vk::MemoryPropertyFlags& memoryPropertyFlags, const vk::DeviceSize& size) const;

        void createStagingBuffer (GameCore::CoreBuffer& result, const vk::DeviceSize& size,
                                  const void* data = nullptr) const;

        void createDeviceBuffer (GameCore::CoreBuffer& result, const vk::BufferUsageFlags& usageFlags,
                                 const vk::DeviceSize& size) const;

        void createSizedUniformBuffer (GameCore::CoreBuffer& result, const vk::DeviceSize& size) const;
    };
}  // namespace GameCore

#endif CORE_BUFFER_MANAGER_HPP
