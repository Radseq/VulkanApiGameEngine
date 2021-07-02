#include "pch.h"
#include "PipelineVertex.hpp"

namespace GraphicCore
{
    void PipelineVertex::update( )
    {
        // vertexInputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        // vertexInputState.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
        // vertexInputState.pVertexBindingDescriptions = bindingDescriptions.data();
        // vertexInputState.pVertexAttributeDescriptions = attributeDescriptions.data();

        Util::updatePointerFromVector (bindingDescriptions, vertexInputState.vertexBindingDescriptionCount,
                                       vertexInputState.pVertexBindingDescriptions);
        Util::updatePointerFromVector (attributeDescriptions, vertexInputState.vertexAttributeDescriptionCount,
                                       vertexInputState.pVertexAttributeDescriptions);
    }

    void PipelineVertex::appendVertexLayout (const VertexLayout& vertexLayout, uint32_t binding,
                                             vk::VertexInputRate rate)
    {
        bindingDescriptions.emplace_back (binding, vertexLayout.stride( ), rate);
        const auto componentsSize = vertexLayout.components.size( );
        attributeDescriptions.reserve (attributeDescriptions.size( ) + componentsSize);
        const auto attributeIndexOffset = uint32_t (attributeDescriptions.size( ));
        for (uint32_t i = 0; i < componentsSize; ++i)
        {
            const auto& component = vertexLayout.components [i];
            const auto  format    = vertexLayout.componentFormat (component);
            const auto  offset    = vertexLayout.offset (i);
            attributeDescriptions.emplace_back (attributeIndexOffset + i, binding, format, offset);
        }
    }

    vk::PipelineVertexInputStateCreateInfo& PipelineVertex::getVertexInput( ) { return vertexInputState; }

    std::vector<vk::VertexInputBindingDescription>& PipelineVertex::getBindingDescriptions( )
    {
        return bindingDescriptions;
    }

    std::vector<vk::VertexInputAttributeDescription>& PipelineVertex::getAttributeDescriptions( )
    {
        return attributeDescriptions;
    }
}  // namespace GraphicCore
