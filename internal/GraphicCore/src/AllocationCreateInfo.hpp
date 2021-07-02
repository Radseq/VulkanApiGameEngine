#ifndef ALLOCATION_CREATE_INFO_HPP
#define ALLOCATION_CREATE_INFO_HPP

#include "pch.h"

namespace GraphicCore {
    class AllocationCreateInfo {
       public:
        vk::MemoryPropertyFlags usage;
        uint32_t                memoryTypeIndex;
        vk::DeviceSize          size;
        bool                    bufferUsageShader {false};
    };
}  // namespace GraphicCore

#endif  // ALLOCATION_CREATE_INFO_HPP
