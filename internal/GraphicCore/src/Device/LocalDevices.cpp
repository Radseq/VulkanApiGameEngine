#include "LocalDevices.hpp"

namespace GraphicCore {
    void LocalDevices::Create (ISetDevice* device) {
        device->PickDevice( );
        device->CreateDevice (surface);

        const std::vector<VulkanDevice*> devices = device->GetLocalDevices( );
        if (!devices.empty( )) { margeLocalDevices (devices); }
    }

    void LocalDevices::margeLocalDevices (const std::vector<VulkanDevice*>& list) {
        for (auto* localDevice : list) { localDevices.push_back (localDevice); }
    }

    LocalDevices::LocalDevices (IVulkanInstance& VulkanInstance, const vk::SurfaceKHR& Surface)
        : surface (Surface)
        , instance (VulkanInstance) {
        Create (new CreateLocalDeviceByGroup (instance));
        Create (new CreateLocalDevice (instance));
    }

    std::vector<VulkanDevice*> LocalDevices::GetLocalDevice( ) const { return localDevices; }

    LocalDevices::~LocalDevices( ) {
        for (auto& localDevice : localDevices)  // It is possible to have more than 255 drivers?
        {
            delete localDevice;
        }
    }
}  // namespace GraphicCore
