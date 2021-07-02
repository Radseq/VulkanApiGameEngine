#ifndef DEVICE_CREATE_INFO_HPP
#define DEVICE_CREATE_INFO_HPP

#include "pch.h"

namespace GraphicCore
{
    class DeviceCreateInfo : public vk::DeviceCreateInfo
    {
        std::vector<vk::DeviceQueueCreateInfo> deviceQueues;
        std::vector<std::vector<float>>        deviceQueuesPriorities;

       public:
        void addQueueFamily (uint32_t queueFamilyIndex, vk::ArrayProxy<float> priorities);
        void addQueueFamily (uint32_t queueFamilyIndex, size_t count = 1);

        void update( );
    };
}  // namespace GraphicCore

#endif DEVICE_CREATE_INFO_HPP