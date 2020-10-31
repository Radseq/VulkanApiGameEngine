#ifndef PIPLINE_VERTEX_HPP
#define PIPLINE_VERTEX_HPP

#include <Util\util.hpp>
#include <vector>
#include <vulkan/vulkan.hpp>

#include "../Model/VertexLayout.hpp"

namespace GameCore {
    class PipelineVertex {
        std::vector<vk::VertexInputBindingDescription>   bindingDescriptions;
        std::vector<vk::VertexInputAttributeDescription> attributeDescriptions;
        vk::PipelineVertexInputStateCreateInfo           vertexInputState;

       public:
        void update( );
        void appendVertexLayout (const VertexLayout& vertexLayout, uint32_t binding = 0,
                                 vk::VertexInputRate rate = vk::VertexInputRate::eVertex);

        vk::PipelineVertexInputStateCreateInfo&           getVertexInput( );
        std::vector<vk::VertexInputBindingDescription>&   getBindingDescriptions( );
        std::vector<vk::VertexInputAttributeDescription>& getAttributeDescriptions( );
    };
}  // namespace GameCore

#endif PIPLINE_VERTEX_HPP
