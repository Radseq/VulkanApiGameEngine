#ifndef ISET_DEVICE_HPP
#define ISET_DEVICE_HPP

#include "pch.h"

namespace GraphicCore
{
    class VulkanDevice;

    class ISetDevice
    {
       public:
        virtual ~ISetDevice( )                                                          = default;
        virtual void                       PickDevice( )                                = 0;
        virtual void                       CreateDevice (const vk::SurfaceKHR& surface) = 0;
        virtual std::vector<VulkanDevice*> GetLocalDevices( )                           = 0;
    };
}  // namespace GraphicCore

#endif ISET_DEVICE_HPP
