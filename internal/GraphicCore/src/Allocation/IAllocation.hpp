#ifndef IALLOCATION_HPP
#define IALLOCATION_HPP

#include "AllocationCreateInfo.hpp"
#include "AllocationProperties.hpp"

namespace GraphicCore {
    class IAllocation {
       public:
        virtual ~IAllocation( )                                                              = default;
        virtual void alloc (AllocationProperties& outAlloc, AllocationCreateInfo createInfo) = 0;
        virtual void free (const AllocationProperties& handle)                               = 0;
    };
}  // namespace GraphicCore

#endif IALLOCATION_HPP
