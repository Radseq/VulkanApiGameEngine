#ifndef ALLOCATION_POOL_HPP
#define ALLOCATION_POOL_HPP

#include <vulkan/vulkan.hpp>

#include "AllocationProperties.hpp"
#include "Device/VulkanDevice.hpp"
#include "IAllocation.hpp"
#include "Util/math.hpp"
#include "Util/util.hpp"

namespace GameCore {
    class VulkanDevice;

    class AllocationPool : public IAllocation {
        struct OffsetSize {
            uint64_t offset;
            uint64_t size;
        };

        struct BlockSpanIndexPair {
            uint32_t blockIdx;
            uint32_t spanIdx;
        };

        struct DeviceMemoryBlock {
            AllocationProperties    mem;
            std::vector<OffsetSize> layout;
            bool                    pageReserved;
        };

        struct MemoryPool {
            std::vector<DeviceMemoryBlock> blocks;
        };

        struct AllocatorState {
            std::vector<size_t> memTypeAllocSizes;
            uint32_t            totalAllocs;

            vk::DeviceSize pageSize;
            vk::DeviceSize memoryBlockMinSize;

            std::vector<MemoryPool> memPools;
        };

        AllocatorState      state = {std::vector<size_t>( ), 0, 0, 0, std::vector<MemoryPool>( )};
        const VulkanDevice& context;

        size_t   allocatedSize (const uint32_t& memoryType) const;
        uint32_t numAllocs( ) const;

        uint32_t addBlockToPool (const vk::DeviceSize& size, const uint32_t& memoryType, bool fitToAlloc, bool bufferUsageShader);
        void     markChunkOfMemoryBlockUsed (const uint32_t& memoryType, const BlockSpanIndexPair& indices,
                                             const vk::DeviceSize& size);
        bool     findFreeChunkForAllocation (BlockSpanIndexPair& outIndexPair, const uint32_t& memoryType,
                                             const vk::DeviceSize& size, const bool& needsWholePage);

       public:
        explicit AllocationPool (const VulkanDevice& Context);

        void alloc (AllocationProperties& outAlloc, AllocationCreateInfo createInfo) override;
        void free (const AllocationProperties& allocationProperties) override;
    };
}  // namespace GameCore

#endif  // ALLOCATION_POOL_HPP
