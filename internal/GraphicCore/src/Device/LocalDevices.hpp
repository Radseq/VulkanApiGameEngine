#ifndef LOCAL_DEVICES_HPP
#define LOCAL_DEVICES_HPP

#include <vulkan/vulkan.hpp>

#include "../Instance/IVulkanInstance.hpp"
#include "../Instance/VulkanInstance.hpp"
#include "CreateLocalDevice.hpp"
#include "CreateLocalDeviceByGroup.hpp"
#include "ISetDevice.hpp"

namespace GraphicCore {
    class ISetDevice;
    class CreateLocalDevice;
    class CreateLocalDeviceByGroup;
    class IVulkanInstance;

    class LocalDevices {
        const vk::SurfaceKHR&      surface;
        IVulkanInstance&           instance;
        std::vector<VulkanDevice*> localDevices;

       public:
        void Create (ISetDevice* device);
        void margeLocalDevices (const std::vector<VulkanDevice*>& list);

        std::vector<VulkanDevice*> GetLocalDevice( ) const;

        LocalDevices (IVulkanInstance& VulkanInstance, const vk::SurfaceKHR& Surface);
        ~LocalDevices( );
    };
}  // namespace GraphicCore

#endif LOCAL_DEVICES_HPP
