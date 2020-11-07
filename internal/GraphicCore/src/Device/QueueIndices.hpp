#ifndef QUEUE_INDICES_HPP
#define QUEUE_INDICES_HPP

#include <vulkan/vulkan.hpp>

namespace GraphicCore {
    class QueueIndices {
       public:
        uint32_t graphics {VK_QUEUE_FAMILY_IGNORED};
        uint32_t present {VK_QUEUE_FAMILY_IGNORED};
        uint32_t transfer {VK_QUEUE_FAMILY_IGNORED};
        uint32_t compute {VK_QUEUE_FAMILY_IGNORED};
        uint32_t sparseBinding {VK_QUEUE_FAMILY_IGNORED};
        uint32_t protectedIndices {VK_QUEUE_FAMILY_IGNORED};
    };
}  // namespace GraphicCore

#endif QUEUE_INDICES_HPP