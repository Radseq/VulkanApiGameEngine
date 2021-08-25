#ifndef CREATE_LOCAL_DEVICE_BY_GROUP_HPP
#define CREATE_LOCAL_DEVICE_BY_GROUP_HPP

#include "ISetDevice.hpp"
#include "IVulkanInstance.hpp"
#include "PhysicalDevice.hpp"
#include "VulkanDevice.hpp"
#include "VulkanInstance.hpp"

namespace GraphicCore
{
    class ISetDevice;
    class PhysicalDevice;
    class IVulkanInstance;

    class CreateLocalDeviceByGroup : public ISetDevice
    {
        std::vector<vk::PhysicalDeviceGroupProperties> physicalDevicesGroupProp;
        std::vector<vk::DeviceGroupDeviceCreateInfo>   devicesGroupCreateInfo;
        std::vector<std::shared_ptr<VulkanDevice>>     LogicalDevices;
        IVulkanInstance&                               instance;

       public:
        CreateLocalDeviceByGroup (IVulkanInstance& VulkanInstance);
        ~CreateLocalDeviceByGroup( ) = default;

        void                                       PickDevice( ) override;
        void                                       CreateDevice (const vk::SurfaceKHR& surface) override;
        std::vector<std::shared_ptr<VulkanDevice>> GetLocalDevices( ) override;
    };
}  // namespace GraphicCore

#endif CREATE_LOCAL_DEVICE_BY_GROUP_HPP
