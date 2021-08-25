#include "pch.h"
#include "CreateLocalDevice.hpp"

namespace GraphicCore
{
    void CreateLocalDevice::PickDevice( )
    {
        for (const auto& group : physicalDevicesGroupProp)
        {
            if (group.physicalDeviceCount == 1)
            {
                for (const auto& physicalDevice : group.physicalDevices)
                {
                    if (physicalDevice != vk::PhysicalDevice( ))
                    {
                        GraphicCore::Util::PassToVec (physicalDevices, physicalDevice);
                    }
                    else
                        break;
                }
            }
        }
    }

    void CreateLocalDevice::CreateDevice (const vk::SurfaceKHR& surface)
    {
        // Use an ordered map to automatically sort candidates by increasing score
        std::multimap<int, PhysicalDevice*, std::greater<int>> scoredPhysicalDevices;

        if (physicalDevices.empty( )) { throw std::runtime_error ("failed to find a suitable GPU!"); }

        for (const auto& physicalDevice : physicalDevices)
        {
            PhysicalDevice* pDevice = new PhysicalDevice (physicalDevice);

            int score = pDevice->getPhysicalDeviceScore( );
            scoredPhysicalDevices.insert (std::make_pair (score, pDevice));
        }

        for (const auto& scoredDevice : scoredPhysicalDevices)
        {
            scoredDevice.second->pickDevice (surface);

            GraphicCore::Util::PassToVec (LogicalDevices, std::make_shared<VulkanDevice> (*scoredDevice.second));

            /*if (SingleGraphicCard)*/ break;
        }
    }

    std::vector<std::shared_ptr<VulkanDevice>> CreateLocalDevice::GetLocalDevices( ) { return LogicalDevices; }

    CreateLocalDevice::CreateLocalDevice (IVulkanInstance& Instance)
        : instance (Instance)
    {
        physicalDevicesGroupProp = instance.getVkInstance( ).enumeratePhysicalDeviceGroups( );
    }
}  // namespace GraphicCore
