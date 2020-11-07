#include "VulkanDevice.hpp"

namespace GraphicCore
{
#ifdef WIN32
    __declspec(thread) vk::CommandPool VulkanDevice::s_cmdPool;
#else
    thread_local vk::CommandPool Context::s_cmdPool;
#endif

    VulkanDevice::VulkanDevice (PhysicalDevice& PhysicalDevice)
        : physicalDevice (PhysicalDevice)
    {
        allocator = std::make_unique<AllocationPool> (*this);
    }

    const std::unique_ptr<IAllocation>& VulkanDevice::getAllocator( ) const { return allocator; }

    /*void VulkanDevice::copyToMemory(const vk::DeviceMemory& memory, const void* data, vk::DeviceSize size,
                                                                    vk::DeviceSize offset) const
    {
            void* mapped = device.mapMemory(memory, offset, size, vk::MemoryMapFlags());
            memcpy(mapped, data, static_cast<size_t>(size));
            device.unmapMemory(memory);
    }*/

    uint32_t VulkanDevice::getMemoryType (const uint32_t& typeBits, const vk::MemoryPropertyFlags& properties) const
    {
        const auto& deviceMemProp = physicalDevice.GetPhysicalDeviceMemoryProp( );

        for (uint32_t i = 0; i < deviceMemProp.memoryTypeCount; ++i)
        {
            if ((typeBits & (1 << i)) && (deviceMemProp.memoryTypes [i].propertyFlags & properties) == properties)
            { return i; }
        }

        throw std::runtime_error ("Unable to find memory type " + to_string (properties));
    }

    void VulkanDevice::Destroy( ) const
    {
        if (s_cmdPool) { device.destroyCommandPool (s_cmdPool); }

        if (graphicsQueue) { graphicsQueue.waitIdle( ); }
        if (presentQueue) { presentQueue.waitIdle( ); }

        device.waitIdle( );
        device.destroy( );
    }

    void VulkanDevice::CreateDevice( )
    {
        // Vulkan device
        device = physicalDevice.GetPhysicalDevice( ).createDevice (physicalDevice.GetDeviceCreateInfo( ));

        // Get the graphics queue
        graphicsQueue = device.getQueue (physicalDevice.ReturnQueueIndices( ).graphics, 0);
        presentQueue  = device.getQueue (physicalDevice.ReturnQueueIndices( ).compute, 0);
    }

    const vk::Device&     VulkanDevice::getVkDevice( ) const { return device; }
    const PhysicalDevice& VulkanDevice::getDevice( ) const { return physicalDevice; }
    const vk::Queue&      VulkanDevice::getGraphicsQueue( ) const { return graphicsQueue; }
    const vk::Queue&      VulkanDevice::getPresentQueue( ) const { return presentQueue; }

    const vk::CommandPool& VulkanDevice::getCommandPool( ) const
    {
        if (!s_cmdPool) throw std::runtime_error ("No found command pool");

        return s_cmdPool;
    }

    void VulkanDevice::CreateCommandPool( ) const
    {
        vk::CommandPoolCreateInfo cmdPoolInfo;
        cmdPoolInfo.queueFamilyIndex = physicalDevice.ReturnQueueIndices( ).graphics;
        // cmdPoolInfo.flags            = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
        s_cmdPool = device.createCommandPool (cmdPoolInfo);
    }

    vk::Format VulkanDevice::getSupportedDepthFormat( ) const
    {
        // Since all depth formats may be optional, we need to find a suitable depth format to use
        // Start with the highest precision packed format
        std::vector<vk::Format> depthFormats = {vk::Format::eD32SfloatS8Uint, vk::Format::eD32Sfloat,
                                                vk::Format::eD24UnormS8Uint, vk::Format::eD16UnormS8Uint,
                                                vk::Format::eD16Unorm};

        return findSupportedFormat (depthFormats, vk::ImageTiling::eOptimal,
                                    vk::FormatFeatureFlagBits::eDepthStencilAttachment);
    }

    vk::Format VulkanDevice::findSupportedFormat (const std::vector<vk::Format>& candidates, vk::ImageTiling tiling,
                                                  const vk::FormatFeatureFlags& features) const
    {
        for (vk::Format format : candidates)
        {
            vk::FormatProperties props;
            props = physicalDevice.GetPhysicalDevice( ).getFormatProperties (format);

            if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features)
            { return format; }
            else if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features)
            {
                return format;
            }
        }
        throw std::runtime_error ("failed to find supported format!");
    }
}  // namespace GraphicCore
