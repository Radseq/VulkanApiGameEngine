#ifndef QUEUE_INDICES_HPP
#define QUEUE_INDICES_HPP

#include <vulkan/vulkan.hpp>

namespace GameCore {
    class QueueIndices {
       public:
        uint32_t graphics {VK_QUEUE_FAMILY_IGNORED};
        uint32_t present {VK_QUEUE_FAMILY_IGNORED};
        uint32_t transfer {VK_QUEUE_FAMILY_IGNORED};
        uint32_t compute {VK_QUEUE_FAMILY_IGNORED};
        uint32_t sparseBinding {VK_QUEUE_FAMILY_IGNORED};
        uint32_t protectedIndices {VK_QUEUE_FAMILY_IGNORED};
    };
}  // namespace GameCore

#endif QUEUE_INDICES_HPP