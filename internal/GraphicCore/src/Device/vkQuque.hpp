#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <vulkan/vulkan.hpp>

namespace GraphicCore {
    class vkQueue {
       public:
        uint32_t    index { };
        vk::Queue   queue;
        std::string queueName;
    };
}  // namespace GraphicCore

#endif QUEUE_HPP