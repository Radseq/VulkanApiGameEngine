#ifndef ISHADER_PIPELINE_HPP
#define ISHADER_PIPELINE_HPP

#include <DescriptorSetLayout.hpp>
#include <VertexLayout.hpp>
#include "pch.h"

class IShaderPipeline
{
   public:
    virtual void CreateGraphicsPipeline (std::vector<std::string>&& shaderPatchName,
                                         const vk::Extent2D& swapChainWindowSize, const vk::RenderPass& renderPass,
                                         const GraphicCore::VertexLayout& vertex) = 0;
    virtual void CreatePipelineLayout (const IShaderDescSet& descSet)             = 0;
    virtual void Bind (const IShaderDescSet& descSet, const vk::CommandBuffer& cmdBufer,
                       const size_t& frameIndex) const                            = 0;
    virtual void Destroy( )                                                       = 0;
};

#endif  // ISHADER_PIPELINE_HPP
