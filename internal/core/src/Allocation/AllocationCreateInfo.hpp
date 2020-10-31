#ifndef ALLOCATION_CREATE_INFO_HPP
#define ALLOCATION_CREATE_INFO_HPP

#include <vulkan/vulkan.hpp>

namespace GameCore {
    class AllocationCreateInfo {
       public:
        vk::MemoryPropertyFlags usage;
        uint32_t                memoryTypeIndex;
        vk::DeviceSize          size;
        bool                    bufferUsageShader {false};
    };
}  // namespace GameCore

#endif  // ALLOCATION_CREATE_INFO_HPP
