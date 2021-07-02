#include "pch.h"
#include "SingleTimeCommandBuffer.hpp"

namespace GraphicCore {
    SingleTimeCommandBuffer::SingleTimeCommandBuffer (const GraphicCore::VulkanDevice& CoreDevice)
        : coreDevice (CoreDevice) { }

    void SingleTimeCommandBuffer::createCommandBuffer (bool begin) {
        //coreDevice.getVkDevice( ).resetCommandPool (coreDevice.getCommandPool( ), { });

        vk::CommandBufferAllocateInfo allocInfo = { };
        allocInfo.level                         = vk::CommandBufferLevel::ePrimary;
        allocInfo.commandPool                   = coreDevice.getCommandPool( );
        allocInfo.commandBufferCount            = 1;

        cmdBuffer = coreDevice.getVkDevice( ).allocateCommandBuffers (allocInfo) [0];
        if (begin) {
            vk::CommandBufferBeginInfo beginInfo = { };
            beginInfo.flags                      = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

            cmdBuffer.begin (beginInfo);
        }
    }

    void SingleTimeCommandBuffer::endSingleTimeCommands (bool free) {
        cmdBuffer.end( );

        vk::SubmitInfo submitInfo     = { };
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers    = &cmdBuffer;

        vk::FenceCreateInfo fenceCreateInfo;
        // fenceCreateInfo.flags = vk::FenceCreateFlagBits::eSignaled; // there is no 0 flag only 1 => eSignaled
        vk::Fence fence = coreDevice.getVkDevice( ).createFence (fenceCreateInfo);

        coreDevice.getGraphicsQueue( ).submit (submitInfo, fence);
        coreDevice.getGraphicsQueue( ).waitIdle( );

        const vk::Result fenceRes = coreDevice.getVkDevice( ).waitForFences (1, &fence, VK_TRUE, DEFAULT_FENCE_TIMEOUT);

        coreDevice.getVkDevice( ).destroyFence (fence);

        if (free) coreDevice.getVkDevice( ).freeCommandBuffers (coreDevice.getCommandPool( ), cmdBuffer);
    }

    const vk::CommandBuffer& SingleTimeCommandBuffer::getCommandBuffer( ) { return cmdBuffer; }
}  // namespace GraphicCore
