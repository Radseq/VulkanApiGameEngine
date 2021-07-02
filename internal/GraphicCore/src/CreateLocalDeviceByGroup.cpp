#include "pch.h"
#include "CreateLocalDeviceByGroup.hpp"

namespace GraphicCore
{
    void CreateLocalDeviceByGroup::PickDevice( )
    {
        for (const auto& group : physicalDevicesGroupProp)
        {
            if (group.physicalDeviceCount > 1)
            {
                vk::DeviceGroupDeviceCreateInfo groupCreateInfo {group.physicalDeviceCount, group.physicalDevices};
                devicesGroupCreateInfo.push_back (groupCreateInfo);
            }
        }
    }

    void CreateLocalDeviceByGroup::CreateDevice (const vk::SurfaceKHR& surface)
    {
        for (const auto& physicalDevicesGroup : devicesGroupCreateInfo)
        {
            // bool IsMainGraphicsPhysticalDevice = (physicalDevicesGroup == devicesGroupCreateInfo.front() ? true :
            // false);
            PhysicalDevice* pDevice               = new PhysicalDevice (physicalDevicesGroup.pPhysicalDevices [0]);
            pDevice->GetDeviceCreateInfo( ).pNext = &physicalDevicesGroup;
            pDevice->pickDevice (surface);

            VulkanDevice* logicalDevice = new VulkanDevice (*pDevice);

            logicalDevice->CreateDevice( );
            LogicalDevices.push_back (logicalDevice);
        }
    }

    std::vector<VulkanDevice*> CreateLocalDeviceByGroup::GetLocalDevices( ) { return LogicalDevices; }

    CreateLocalDeviceByGroup::CreateLocalDeviceByGroup (IVulkanInstance& Instance)
        : instance (Instance)
    {
        physicalDevicesGroupProp = instance.getVkInstance( ).enumeratePhysicalDeviceGroups( );
    }
}  // namespace GraphicCore
