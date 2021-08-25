#ifndef CREATE_LOCAL_DEVICE_HPP
#define CREATE_LOCAL_DEVICE_HPP

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

    class CreateLocalDevice : public ISetDevice
    {
        std::vector<vk::PhysicalDeviceGroupProperties> physicalDevicesGroupProp;
        std::vector<vk::PhysicalDevice>                physicalDevices;
        IVulkanInstance&                               instance;
        std::vector<std::shared_ptr<VulkanDevice>>     LogicalDevices;

       public:
        CreateLocalDevice (IVulkanInstance& Instance);
        ~CreateLocalDevice( ) = default;

        void                                       PickDevice( ) override;
        void                                       CreateDevice (const vk::SurfaceKHR& surface) override;
        std::vector<std::shared_ptr<VulkanDevice>> GetLocalDevices( ) override;
    };
}  // namespace GraphicCore

#endif CREATE_LOCAL_DEVICE_HPP
