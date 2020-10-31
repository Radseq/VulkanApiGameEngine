#ifndef ISET_DEVICE_HPP
#define ISET_DEVICE_HPP

#include <vulkan/vulkan.hpp>

#include "VulkanDevice.hpp"

namespace GameCore {
    class VulkanDevice;

    class ISetDevice {
       public:
        virtual ~ISetDevice( )                                                          = default;
        virtual void                       PickDevice( )                                = 0;
        virtual void                       CreateDevice (const vk::SurfaceKHR& surface) = 0;
        virtual std::vector<VulkanDevice*> GetLocalDevices( )                           = 0;
    };
}  // namespace GameCore

#endif ISET_DEVICE_HPP
