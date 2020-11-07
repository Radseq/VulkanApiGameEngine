#ifndef ALLOCATION_PASS_THROUGH_HPP
#define ALLOCATION_PASS_THROUGH_HPP

#include <vulkan/vulkan.hpp>

#include "AllocationProperties.hpp"
#include "Device/VulkanDevice.hpp"
#include "IAllocation.hpp"

namespace GraphicCore {
    class VulkanDevice;

    class AllocationPassThrough : public IAllocation {
        struct AllocatorState {
            size_t*  memTypeAllocSizes;
            uint32_t totalAllocs;
        };

        AllocatorState      state {nullptr, 0};
        const VulkanDevice& context;

        size_t&  allocatedSize (uint32_t memoryType) const;
        uint32_t numAllocs( ) const;

       public:
        explicit AllocationPassThrough (const VulkanDevice& Context);
        void alloc (AllocationProperties& outAlloc, AllocationCreateInfo createInfo) override;
        void free (const AllocationProperties& allocationProperties) override;
    };
}  // namespace GraphicCore

#endif  // ALLOCATION_PASS_THROUGH_HPP
