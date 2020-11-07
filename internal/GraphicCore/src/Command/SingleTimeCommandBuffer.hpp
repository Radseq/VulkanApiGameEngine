#ifndef SINGLE_TIME_CMD_BUFFER_HPP
#define SINGLE_TIME_CMD_BUFFER_HPP

#include <Device\VulkanDevice.hpp>
#include <vulkan/vulkan.hpp>

namespace GraphicCore {
    class SingleTimeCommandBuffer {
        vk::CommandBuffer             cmdBuffer;
        const uint64_t                DEFAULT_FENCE_TIMEOUT {100000000000};
        const GraphicCore::VulkanDevice& coreDevice;

       public:
        SingleTimeCommandBuffer (const GraphicCore::VulkanDevice& CoreDevice);

        void createCommandBuffer (bool begin);
        void endSingleTimeCommands (bool free);

        const vk::CommandBuffer& getCommandBuffer( );
    };
}  // namespace GraphicCore

#endif  // SINGLE_TIME_CMD_BUFFER_HPP
