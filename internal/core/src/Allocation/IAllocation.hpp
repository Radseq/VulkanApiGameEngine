#ifndef IALLOCATION_HPP
#define IALLOCATION_HPP

#include "AllocationCreateInfo.hpp"
#include "AllocationProperties.hpp"

namespace GameCore {
    class IAllocation {
       public:
        virtual ~IAllocation( )                                                              = default;
        virtual void alloc (AllocationProperties& outAlloc, AllocationCreateInfo createInfo) = 0;
        virtual void free (const AllocationProperties& handle)                               = 0;
    };
}  // namespace GameCore

#endif IALLOCATION_HPP
