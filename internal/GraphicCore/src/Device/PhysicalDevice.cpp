#include "PhysicalDevice.hpp"

namespace GraphicCore {
    void PhysicalDevice::BuildDeviceCreateInfo( ) {
        deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

        deviceCreateInfo.addQueueFamily (queueIndices.graphics, 1
                                         /*queueFamilyProperties[queueIndices.graphics].queueCount*/);
        if (queueIndices.compute != VK_QUEUE_FAMILY_IGNORED && queueIndices.compute != queueIndices.graphics) {
            deviceCreateInfo.addQueueFamily (queueIndices.compute, 1
                                             /*queueFamilyProperties[queueIndices.compute].queueCount*/);
        }
        if (queueIndices.transfer != VK_QUEUE_FAMILY_IGNORED && queueIndices.transfer != queueIndices.graphics &&
            queueIndices.transfer != queueIndices.compute) {
            deviceCreateInfo.addQueueFamily (queueIndices.transfer, 1
                                             /*queueFamilyProperties[queueIndices.transfer].queueCount*/);
        }
        deviceCreateInfo.update( );

        // enable the debug marker extension if it is present (likely meaning a debugging tool is present)
        if (isDeviceExtensionPresent (VK_EXT_DEBUG_MARKER_EXTENSION_NAME)) {
            enabledExtensions.push_back (VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
            enableDebugMarkers = true;
        }

        if (isDeviceExtensionPresent (VK_KHR_SWAPCHAIN_EXTENSION_NAME)) {
            enabledExtensions.push_back (VK_KHR_SWAPCHAIN_EXTENSION_NAME);
        }

        if (!enabledExtensions.empty( )) {
            deviceCreateInfo.enabledExtensionCount   = uint32_t (enabledExtensions.size( ));
            deviceCreateInfo.ppEnabledExtensionNames = enabledExtensions.data( );
        }
    }

    void PhysicalDevice::rateDeviceSore( ) {
        // Discrete GPUs have a significant performance advantage
        if (deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) { score += 1000; }

        // Maximum possible size of textures affects graphics quality
        score += deviceProperties.limits.maxImageDimension2D;

        // Application can't function without geometry shaders
        if (!deviceFeatures.geometryShader) { score = 0; }
    }
    /// <summary>
    /// validate if queue can present to window surface
    /// because in findQueue func validate not working
    /// </summary>
    /// <param name="surface"></param>
    void PhysicalDevice::validateDeviceSupportQueueIndices (const vk::SurfaceKHR& surface) {
        vk::Bool32 isDeviceSupportQueueIndices = physicalDevice.getSurfaceSupportKHR (queueIndices.compute, surface);

        if (!isDeviceSupportQueueIndices)
            throw std::runtime_error ("physical device surface not support queue family index " + queueIndices.compute);
    }

    PhysicalDevice::PhysicalDevice (const vk::PhysicalDevice& _physicalDevice)
        : physicalDevice (_physicalDevice)
        ,
        // queueFamilyProperties(new std::vector<vk::QueueFamilyProperties>(physicalDevice.getQueueFamilyProperties())),
        queueFamilyProperties (physicalDevice.getQueueFamilyProperties( ))
        , deviceProperties (physicalDevice.getProperties( ))
        , deviceFeatures (physicalDevice.getFeatures( ))
        , deviceMemoryProperties (physicalDevice.getMemoryProperties( )) {
        // queueFamilyProperties = new std::vector<vk::QueueFamilyProperties>
        // (physicalDevice.getQueueFamilyProperties());
        setDeviceExtensionNames( );
        rateDeviceSore( );
    }

    // PhysicalDevice::~PhysicalDevice()
    //{
    // delete(queueFamilyProperties);
    //}

    std::vector<vk::ExtensionProperties> PhysicalDevice::GetDeviceExtensions( ) const {
        return physicalDevice.enumerateDeviceExtensionProperties( );
    }

    void PhysicalDevice::pickDevice (const vk::SurfaceKHR& surface) {
        queueIndices.graphics = findQueue (vk::QueueFlagBits::eGraphics, surface);
        queueIndices.compute  = findQueue (vk::QueueFlagBits::eCompute);
        queueIndices.transfer = findQueue (vk::QueueFlagBits::eTransfer);

        queueIndices.sparseBinding    = findQueue (vk::QueueFlagBits::eSparseBinding);
        queueIndices.protectedIndices = findQueue (vk::QueueFlagBits::eProtected);

        validateDeviceSupportQueueIndices (surface);

        BuildDeviceCreateInfo( );
    }

    /*vk::PhysicalDevice& PhysicalDevice::GetPhysicalDevice()
    {
            return physicalDevice;
    }*/

    const vk::PhysicalDevice&         PhysicalDevice::GetPhysicalDevice( ) const { return physicalDevice; }
    const vk::PhysicalDeviceFeatures& PhysicalDevice::GetPhysicalDeviceFeatures( ) const { return deviceFeatures; }
    const vk::PhysicalDeviceMemoryProperties& PhysicalDevice::GetPhysicalDeviceMemoryProp( ) const {
        return deviceMemoryProperties;
    }
    const QueueIndices&              PhysicalDevice::ReturnQueueIndices( ) const { return queueIndices; }
    const vk::QueueFamilyProperties& PhysicalDevice::GetQueFamilyProp (const size_t& index) const {
        // return queueFamilyProperties->at(index);
        return queueFamilyProperties [index];
    }

    const vk::PhysicalDeviceProperties& PhysicalDevice::GetDeviceProperties( ) const { return deviceProperties; }
    DeviceCreateInfo&                   PhysicalDevice::GetDeviceCreateInfo( ) { return deviceCreateInfo; }
    const int&                          PhysicalDevice::getPhysicalDeviceScore( ) const { return score; }

    /* PhysicalDevice::QueueFamilyIndices PhysicalDevice::findQueueFamilies(const vk::QueueFlags& desiredFlags, const
    vk::SurfaceKHR& presentSurface) { QueueFamilyIndices indices;

            size_t queueFamilyCount = queueFamilyProperties.size();

            int i = 0;
            for (const auto& queueFamily : queueFamilyProperties) {
                    if (queueFamily.queueCount > 0 && (queueFamily.queueFlags & desiredFlags)) {
                            indices.graphicsFamily = i;
                    }

                    if (queueFamily.queueCount > 0 && VK_FALSE == physicalDevice.getSurfaceSupportKHR(i,
    presentSurface)) { indices.presentFamily = i;
                    }

                    if (indices.isComplete()) {
                            break;
                    }

                    i++;
            }

            return indices;
    }*/

    uint32_t PhysicalDevice::findQueue (const vk::QueueFlags& desiredFlags, const vk::SurfaceKHR& presentSurface) {
        uint32_t     bestMatch {VK_QUEUE_FAMILY_IGNORED};
        VkQueueFlags bestMatchExtraFlags {VK_QUEUE_FLAG_BITS_MAX_ENUM};
        const size_t queueCount = queueFamilyProperties.size( );

        for (uint32_t i = 0; i < queueCount; ++i) {
            auto currentFlags = queueFamilyProperties [i].queueFlags;  // queueFamilyProperties->at(i).queueFlags;
            // Doesn't contain the required flags, skip it
            if (!(currentFlags & desiredFlags)) { continue; }

            if (presentSurface && /*VK_FALSE*/ VK_TRUE == physicalDevice.getSurfaceSupportKHR (i, presentSurface) &&
                queueIndices.present ==
                    UINT32_MAX)  // dont work somehow thats why created validateDeviceSupportQueueIndices func
            {
                queueIndices.present = i;
                // continue;
            }

            const VkQueueFlags currentExtraFlags = (currentFlags & ~desiredFlags).operator VkQueueFlags( );

            // If we find an exact match, return immediately
            if (0 == currentExtraFlags) { return i; }

            if (bestMatch == VK_QUEUE_FAMILY_IGNORED || currentExtraFlags < bestMatchExtraFlags) {
                bestMatch           = i;
                bestMatchExtraFlags = currentExtraFlags;
            }
        }

        return bestMatch;
    }

    void PhysicalDevice::setDeviceExtensionNames( ) {
        for (const auto& ext : GetDeviceExtensions( )) {
            std::string str (ext.extensionName._Elems, strnlen (ext.extensionName._Elems, ext.extensionName.size( )));
            _extensionNames.insert (str);
        }
    }

    bool PhysicalDevice::isDeviceExtensionPresent (const std::string& extension) const {
        return _extensionNames.count (extension) != 0;
    }

    const vk::SampleCountFlagBits PhysicalDevice::getMaxUsableSampleCount( ) const {
        vk::PhysicalDeviceProperties physicalDeviceProperties = GetDeviceProperties( );

        vk::SampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts &
                                      physicalDeviceProperties.limits.framebufferDepthSampleCounts;
        if (counts & vk::SampleCountFlagBits::e64) { return vk::SampleCountFlagBits::e64; }
        if (counts & vk::SampleCountFlagBits::e32) { return vk::SampleCountFlagBits::e32; }
        if (counts & vk::SampleCountFlagBits::e16) { return vk::SampleCountFlagBits::e16; }
        if (counts & vk::SampleCountFlagBits::e8) { return vk::SampleCountFlagBits::e8; }
        if (counts & vk::SampleCountFlagBits::e4) { return vk::SampleCountFlagBits::e4; }
        if (counts & vk::SampleCountFlagBits::e2) { return vk::SampleCountFlagBits::e2; }
        return vk::SampleCountFlagBits::e1;
    }
}  // namespace GraphicCore
