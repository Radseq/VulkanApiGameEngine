#ifndef CREATE_LOCAL_DEVICE_HPP
#define CREATE_LOCAL_DEVICE_HPP

#include <map>
#include <vulkan/vulkan.hpp>

#include "../../../../src/Util/Configuration.hpp"
#include "../Instance/IVulkanInstance.hpp"
#include "../Instance/VulkanInstance.hpp"
#include "ISetDevice.hpp"

namespace GraphicCore {
    class ISetDevice;
    class PhysicalDevice;
    class IVulkanInstance;

    class CreateLocalDevice : public ISetDevice {
        std::vector<vk::PhysicalDeviceGroupProperties> physicalDevicesGroupProp;
        std::vector<vk::PhysicalDevice>                physicalDevices;
        IVulkanInstance&                               instance;
        std::vector<VulkanDevice*>                     LogicalDevices;

        const bool ONE_GRAPHIC_CARD = Configuration::getInstance( )->GetConfigBoolOption ("UseOneGraphicsCard");

       public:
        CreateLocalDevice (IVulkanInstance& Instance);
        ~CreateLocalDevice( ) = default;

        void                       PickDevice( ) override;
        void                       CreateDevice (const vk::SurfaceKHR& surface) override;
        std::vector<VulkanDevice*> GetLocalDevices( ) override;
    };
}  // namespace GraphicCore

#endif CREATE_LOCAL_DEVICE_HPP
