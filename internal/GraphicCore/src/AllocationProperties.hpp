#ifndef ALLOCATION_PROPERTIES_HPP
#define ALLOCATION_PROPERTIES_HPP

#include "pch.h"

namespace GraphicCore
{
    class AllocationProperties
    {
       public:
        vk::DeviceMemory handle;
        uint32_t         type;
        uint32_t         id;
        vk::DeviceSize   size;
        vk::DeviceSize   offset;
    };
}  // namespace GraphicCore

#endif ALLOCATION_PROPERTIES_HPP
