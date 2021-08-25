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
                GraphicCore::Util::PassToVec (devicesGroupCreateInfo, group.physicalDeviceCount, group.physicalDevices);
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

            GraphicCore::Util::PassToVec (LogicalDevices, std::make_shared<VulkanDevice> (*pDevice));
        }
    }

    std::vector<std::shared_ptr<VulkanDevice>> CreateLocalDeviceByGroup::GetLocalDevices( ) { return LogicalDevices; }

    CreateLocalDeviceByGroup::CreateLocalDeviceByGroup (IVulkanInstance& Instance)
        : instance (Instance)
    {
        physicalDevicesGroupProp = instance.getVkInstance( ).enumeratePhysicalDeviceGroups( );
    }
}  // namespace GraphicCore
