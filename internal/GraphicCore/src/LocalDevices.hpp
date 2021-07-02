#ifndef LOCAL_DEVICES_HPP
#define LOCAL_DEVICES_HPP

#include "CreateLocalDevice.hpp"
#include "CreateLocalDeviceByGroup.hpp"
#include "ILocalDevices.hpp"
#include "ISetDevice.hpp"
#include "IVulkanInstance.hpp"
#include "VulkanInstance.hpp"

namespace GraphicCore
{
    class ISetDevice;
    class CreateLocalDevice;
    class CreateLocalDeviceByGroup;
    class IVulkanInstance;

    class LocalDevices : public ILocalDevices
    {
        const vk::SurfaceKHR&      surface;
        IVulkanInstance&           instance;
        std::vector<VulkanDevice*> localDevices;
        void                       margeLocalDevices (const std::vector<VulkanDevice*>& list);

       public:
        void Create (ISetDevice* device) override;

        std::vector<VulkanDevice*> GetLocalDevice( ) const override;

        LocalDevices (IVulkanInstance& VulkanInstance, const vk::SurfaceKHR& Surface);
        ~LocalDevices( );
    };
}  // namespace GraphicCore

#endif LOCAL_DEVICES_HPP
