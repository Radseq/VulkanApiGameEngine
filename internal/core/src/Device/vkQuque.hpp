#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <vulkan/vulkan.hpp>

namespace GameCore {
    class vkQueue {
       public:
        uint32_t    index { };
        vk::Queue   queue;
        std::string queueName;
    };
}  // namespace GameCore

#endif QUEUE_HPP