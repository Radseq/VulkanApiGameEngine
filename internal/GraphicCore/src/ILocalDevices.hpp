#ifndef ILOCAL_DEVICES_HPP
#define ILOCAL_DEVICES_HPP

#include "ISetDevice.hpp"
#include "IVulkanInstance.hpp"

namespace GraphicCore
{
    class ISetDevice;
    class VulkanDevice;

    class ILocalDevices
    {
       public:
        virtual void Create (ISetDevice* device) = 0;

        virtual std::vector<std::shared_ptr<VulkanDevice>> GetLocalDevice( ) const = 0;
    };
}  // namespace GraphicCore

#endif LOCAL_DEVICES_HPP
