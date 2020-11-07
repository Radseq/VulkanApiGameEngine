#include "AllocationPassThrough.hpp"

namespace GraphicCore {
    void AllocationPassThrough::alloc (AllocationProperties& outAlloc, AllocationCreateInfo createInfo) {
        state.totalAllocs++;
        state.memTypeAllocSizes [createInfo.memoryTypeIndex] += createInfo.size;

        vk::MemoryAllocateInfo info;  // vkh::memoryAllocateInfo(newPoolSize, memoryType);
        info.allocationSize = createInfo.size;
        // Get the memory type index that supports host visible memory access
        // Most implementations offer multiple memory types and selecting the
        // correct one to allocate memory from is important
        info.memoryTypeIndex = createInfo.memoryTypeIndex;

        if (createInfo.bufferUsageShader) {
            vk::MemoryAllocateFlagsInfoKHR allocFlagsInfo { };
            allocFlagsInfo.flags = vk::MemoryAllocateFlagBits::eDeviceAddress;
            info.pNext           = &allocFlagsInfo;
        }

        try {
            outAlloc.handle = context.getVkDevice( ).allocateMemory (info);
            outAlloc.size   = createInfo.size;
            outAlloc.type   = createInfo.memoryTypeIndex;
            outAlloc.offset = 0;
        } catch (std::exception const& e) {
            std::string errMsg;
            util::constCharToString (errMsg, e.what( ));
            throw std::runtime_error ("failed create memory " + errMsg);
        }
    }

    void AllocationPassThrough::free (const AllocationProperties& allocationProperties) {
        state.totalAllocs--;
        state.memTypeAllocSizes [allocationProperties.type] -= allocationProperties.size;

        context.getVkDevice( ).freeMemory (allocationProperties.handle);
    }

    size_t& AllocationPassThrough::allocatedSize (uint32_t memoryType) const {
        return state.memTypeAllocSizes [memoryType];
    }

    uint32_t AllocationPassThrough::numAllocs( ) const { return state.totalAllocs; }

    AllocationPassThrough::AllocationPassThrough (const VulkanDevice& Context)
        : context (Context) {
        const vk::PhysicalDeviceMemoryProperties memProperties = context.getDevice( ).GetPhysicalDeviceMemoryProp( );

        state.memTypeAllocSizes = static_cast<size_t*> (calloc (1, sizeof (size_t) * memProperties.memoryTypeCount));
    }
}  // namespace GraphicCore
