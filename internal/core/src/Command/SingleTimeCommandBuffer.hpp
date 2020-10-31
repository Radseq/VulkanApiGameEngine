#ifndef SINGLE_TIME_CMD_BUFFER_HPP
#define SINGLE_TIME_CMD_BUFFER_HPP

#include <Device\VulkanDevice.hpp>
#include <vulkan/vulkan.hpp>

namespace GameCore {
    class SingleTimeCommandBuffer {
        vk::CommandBuffer             cmdBuffer;
        const uint64_t                DEFAULT_FENCE_TIMEOUT {100000000000};
        const GameCore::VulkanDevice& coreDevice;

       public:
        SingleTimeCommandBuffer (const GameCore::VulkanDevice& CoreDevice);

        void createCommandBuffer (bool begin);
        void endSingleTimeCommands (bool free);

        const vk::CommandBuffer& getCommandBuffer( );
    };
}  // namespace GameCore

#endif  // SINGLE_TIME_CMD_BUFFER_HPP
