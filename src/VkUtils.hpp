#ifndef VK_GAME_HELPERS_HPP
#define VK_GAME_HELPERS_HPP

#include "pch.h"

namespace VulkanGame
{
    namespace VkHelpers
    {
        inline vk::Viewport viewport (const glm::uvec2& size, float minDepth = 0, float maxDepth = 1)
        {
            vk::Viewport viewport;
            viewport.width    = static_cast<float> (size.x);
            viewport.height   = static_cast<float> (size.y);
            viewport.minDepth = minDepth;
            viewport.maxDepth = maxDepth;
            return viewport;
        }

        inline vk::Rect2D rect2D (const glm::uvec2& size, const glm::ivec2& offset = glm::ivec2 (0))
        {
            vk::Rect2D rect2D;
            rect2D.extent.width  = size.x;
            rect2D.extent.height = size.y;
            rect2D.offset.x      = offset.x;
            rect2D.offset.y      = offset.y;
            return rect2D;
        }

        inline vk::ClearColorValue clearColor (const glm::vec4& v = glm::vec4 (0))
        {
            vk::ClearColorValue result;
            memcpy (&result.float32, &v, sizeof (result.float32));
            return result;
        }
    }  // namespace VkHelpers
}  // namespace VulkanGame

#endif /* VK_GAME_HELPERS_HPP */