#ifndef PHYSICAL_DEVICE_HPP
#define PHYSICAL_DEVICE_HPP

#include <Device\QueueIndices.hpp>
#include <Util\vkHelpers.hpp>
#include <set>
#include <vulkan/vulkan.hpp>

#include "../Instance/VulkanInstance.hpp"
#include "DeviceCreateInfo.hpp"

namespace GraphicCore {
    class VulkanInstance;
    class DeviceCreateInfo;
    class QueueIndices;

    // using DeviceFeaturesPickerFunction = std::function<vk::PhysicalDeviceFeatures(const vk::PhysicalDevice&, const
    // vk::PhysicalDeviceFeatures&)>;

    class PhysicalDevice {
        int score = 0;

        const vk::PhysicalDevice&                    physicalDevice;  // Physical device (GPU) that Vulkan will use
        const std::vector<vk::QueueFamilyProperties> queueFamilyProperties;
        const vk::PhysicalDeviceProperties
            deviceProperties;  // Stores physical device properties (for e.g. checking device limits)
        const vk::PhysicalDeviceFeatures
            deviceFeatures;  // Stores phyiscal device features (for e.g. checking if a feature is available)
        const vk::PhysicalDeviceMemoryProperties
            deviceMemoryProperties;  // Stores all available memory (type) properties for the physical device

        bool         enableDebugMarkers {false};
        QueueIndices queueIndices { };

        // QueueFamilyIndices PhysicalDevice::findQueueFamilies(const vk::QueueFlags& desiredFlags, const
        // vk::SurfaceKHR& presentSurface);

        [[nodiscard]] uint32_t findQueue (const vk::QueueFlags& desiredFlags,
                                          const vk::SurfaceKHR& presentSurface = nullptr);

        void                     setDeviceExtensionNames( );
        std::set<std::string>    _extensionNames;
        [[nodiscard]] bool       isDeviceExtensionPresent (const std::string& extension) const;
        std::vector<const char*> enabledExtensions;
        void                     BuildDeviceCreateInfo( );
        DeviceCreateInfo         deviceCreateInfo { };
        void                     rateDeviceSore( );
        void                     validateDeviceSupportQueueIndices (const vk::SurfaceKHR& surface);

       public:
        PhysicalDevice (const vk::PhysicalDevice& _physicalDevice);

        [[nodiscard]] std::vector<vk::ExtensionProperties> GetDeviceExtensions( ) const;
        void                                               pickDevice (const vk::SurfaceKHR& surface);
        // vk::PhysicalDevice& GetPhysicalDevice();
        const vk::PhysicalDevice&                 GetPhysicalDevice( ) const;
        const vk::PhysicalDeviceFeatures&         GetPhysicalDeviceFeatures( ) const;
        const vk::PhysicalDeviceMemoryProperties& GetPhysicalDeviceMemoryProp( ) const;
        const QueueIndices&                       ReturnQueueIndices( ) const;
        const vk::QueueFamilyProperties&          GetQueFamilyProp (const size_t& index) const;
        const vk::PhysicalDeviceProperties&       GetDeviceProperties( ) const;
        DeviceCreateInfo&                         GetDeviceCreateInfo( );
        const int&                                getPhysicalDeviceScore( ) const;
        const vk::SampleCountFlagBits             getMaxUsableSampleCount( ) const;
    };
}  // namespace GraphicCore

#endif PHYSICAL_DEVICE_HPP
