#ifndef CREATE_LOCAL_DEVICE_BY_GROUP_HPP
#define CREATE_LOCAL_DEVICE_BY_GROUP_HPP

#include <vulkan/vulkan.hpp>

#include "../Instance/IVulkanInstance.hpp"
#include "../Instance/VulkanInstance.hpp"
#include "ISetDevice.hpp"

namespace GameCore {
    class ISetDevice;
    class PhysicalDevice;
    class IVulkanInstance;

    class CreateLocalDeviceByGroup : public ISetDevice {
        std::vector<vk::PhysicalDeviceGroupProperties> physicalDevicesGroupProp;
        std::vector<vk::DeviceGroupDeviceCreateInfo>   devicesGroupCreateInfo;
        std::vector<VulkanDevice*>                     LogicalDevices;
        IVulkanInstance&                               instance;

       public:
        CreateLocalDeviceByGroup (IVulkanInstance& VulkanInstance);
        ~CreateLocalDeviceByGroup( ) = default;

        void                       PickDevice( ) override;
        void                       CreateDevice (const vk::SurfaceKHR& surface) override;
        std::vector<VulkanDevice*> GetLocalDevices( ) override;
    };
}  // namespace GameCore

#endif CREATE_LOCAL_DEVICE_BY_GROUP_HPP
