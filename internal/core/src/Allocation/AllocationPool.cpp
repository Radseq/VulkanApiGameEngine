#include "AllocationPool.hpp"

namespace GameCore {
    void AllocationPool::alloc (AllocationProperties& outAlloc, AllocationCreateInfo createInfo) {
        const uint32_t       memoryType = createInfo.memoryTypeIndex;
        const vk::DeviceSize size       = createInfo.size;

        MemoryPool& pool = state.memPools [memoryType];
        // make sure we always alloc a multiple of pageSize
        const vk::DeviceSize requestedAllocSize = ((size / state.pageSize) + 1) * state.pageSize;
        state.memTypeAllocSizes [memoryType] += requestedAllocSize;

        BlockSpanIndexPair location { };

        const bool needsOwnPage = createInfo.usage != vk::MemoryPropertyFlagBits::eDeviceLocal;
        const bool found        = findFreeChunkForAllocation (location, memoryType, requestedAllocSize, needsOwnPage);

        if (!found) { location = {addBlockToPool (requestedAllocSize, memoryType, needsOwnPage, createInfo.bufferUsageShader), 0}; }

        pool.blocks [location.blockIdx].pageReserved = needsOwnPage;

        outAlloc.handle = pool.blocks [location.blockIdx].mem.handle;
        outAlloc.size   = size;

        outAlloc.offset = pool.blocks [location.blockIdx].layout [location.spanIdx].offset;
        outAlloc.type   = memoryType;
        outAlloc.id     = location.blockIdx;

        markChunkOfMemoryBlockUsed (memoryType, location, requestedAllocSize);
    }

    void AllocationPool::free (const AllocationProperties& allocationProperties) {
        const vk::DeviceSize requestedAllocSize = ((allocationProperties.size / state.pageSize) + 1) * state.pageSize;

        const OffsetSize span = {allocationProperties.offset, requestedAllocSize};

        MemoryPool& pool                                   = state.memPools [allocationProperties.type];
        pool.blocks [allocationProperties.id].pageReserved = false;

        bool found = false;

        const size_t numLayoutMems = pool.blocks [allocationProperties.id].layout.size( );
        for (size_t j = 0; j < numLayoutMems; ++j) {
            if (pool.blocks [allocationProperties.id].layout [j].offset ==
                requestedAllocSize + allocationProperties.offset) {
                pool.blocks [allocationProperties.id].layout [j].offset = allocationProperties.offset;
                pool.blocks [allocationProperties.id].layout [j].size += requestedAllocSize;
                found = true;
                break;
            }
        }

        if (!found) {
            state.memPools [allocationProperties.type].blocks [allocationProperties.id].layout.push_back (span);
            state.memTypeAllocSizes [allocationProperties.type] -= requestedAllocSize;
            context.getVkDevice( ).freeMemory (allocationProperties.handle);
        }
    }

    uint32_t AllocationPool::addBlockToPool (const vk::DeviceSize& size, const uint32_t& memoryType, bool fitToAlloc,
                                             bool bufferUsageShader) {
        vk::DeviceSize newPoolSize = util::checkMaxUint64Size (size * 2);
        newPoolSize                = newPoolSize < state.memoryBlockMinSize ? state.memoryBlockMinSize : newPoolSize;

        vk::MemoryAllocateInfo info;  // vkh::memoryAllocateInfo(newPoolSize, memoryType);
        info.allocationSize = newPoolSize;
        // Get the memory type index that supports host visible memory access
        // Most implementations offer multiple memory types and selecting the
        // correct one to allocate memory from is important
        info.memoryTypeIndex = memoryType;

        if (bufferUsageShader) {
            vk::MemoryAllocateFlagsInfoKHR allocFlagsInfo { };
            allocFlagsInfo.flags = vk::MemoryAllocateFlagBits::eDeviceAddress;
            info.pNext           = &allocFlagsInfo;
        }

        DeviceMemoryBlock newBlock = { };

        try {
            newBlock.mem.handle = context.getVkDevice( ).allocateMemory (info);
        } catch (std::exception const& e) {
            std::string errMsg;
            util::constCharToString (errMsg, e.what( ));
            throw std::runtime_error ("failed create memory " + errMsg);
        }

        newBlock.mem.type = memoryType;
        newBlock.mem.size = newPoolSize;

        MemoryPool& pool = state.memPools [memoryType];
        pool.blocks.push_back (newBlock);
        pool.blocks [pool.blocks.size( ) - 1].layout.push_back ({0, newPoolSize});

        state.totalAllocs++;

        const uint32_t returnValue = util::to_uint_32_t (pool.blocks.size( )) - 1;
        return returnValue;
    }

    void AllocationPool::markChunkOfMemoryBlockUsed (const uint32_t& memoryType, const BlockSpanIndexPair& indices,
                                                     const vk::DeviceSize& size) {
        MemoryPool& pool = state.memPools [memoryType];

        pool.blocks [indices.blockIdx].layout [indices.spanIdx].offset += size;
        pool.blocks [indices.blockIdx].layout [indices.spanIdx].size -= size;
    }

    bool AllocationPool::findFreeChunkForAllocation (BlockSpanIndexPair& outIndexPair, const uint32_t& memoryType,
                                                     const vk::DeviceSize& size, const bool& needsWholePage) {
        MemoryPool& pool = state.memPools [memoryType];

        /*uint32_t blocksIterator = 0;
        uint32_t layoutIterator = 0;
        for (const auto& i : pool.blocks)
        {
                for (const auto& j : i.layout)
                {
                        const bool validOffset = needsWholePage ? j.offset == 0 : true;
                        if (j.size >= size && validOffset)
                        {
                                outIndexPair.blockIdx = blocksIterator;
                                outIndexPair.spanIdx = layoutIterator;
                                return true;
                        }
                        ++layoutIterator;
                }
                ++blocksIterator;
        }*/

        // this code is faster by 5ms in 1000000 iter loop
        for (uint32_t i = 0; i < pool.blocks.size( ); ++i) {
            for (uint32_t j = 0; j < pool.blocks [i].layout.size( ); ++j) {
                const bool validOffset = needsWholePage ? pool.blocks [i].layout [j].offset == 0 : true;
                if (pool.blocks [i].layout [j].size >= size && validOffset) {
                    outIndexPair.blockIdx = i;
                    outIndexPair.spanIdx  = j;
                    return true;
                }
            }
        }
        // auto start_index_32_2 = std::chrono::high_resolution_clock::now();
        // loop 1000000 iter
        // code
        // end loop
        // auto end_index_32_2 = std::chrono::high_resolution_clock::now();
        // auto time_index_32_2 = std::chrono::duration_cast<std::chrono::milliseconds>(end_index_32_2 -
        // start_index_32_2).count();

        return false;
    }

    AllocationPool::AllocationPool (const VulkanDevice& Context)
        : context (Context) {
        const vk::PhysicalDeviceMemoryProperties memProperties = context.getDevice( ).GetPhysicalDeviceMemoryProp( );

        state.memTypeAllocSizes.resize (memProperties.memoryTypeCount);
        state.memPools.resize (memProperties.memoryTypeCount);

        state.pageSize           = context.getDevice( ).GetDeviceProperties( ).limits.bufferImageGranularity;
        state.memoryBlockMinSize = util::checkMaxUint64Size (state.pageSize * 10);
    }

    size_t AllocationPool::allocatedSize (const uint32_t& memoryType) const {
        return state.memTypeAllocSizes [memoryType];
    }

    uint32_t AllocationPool::numAllocs( ) const { return state.totalAllocs; }
}  // namespace GameCore
