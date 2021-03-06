#ifndef ENTITY_PIPELINE_HPP
#define ENTITY_PIPELINE_HPP

#include <DescriptorSets.hpp>
#include <Pipeline.hpp>
#include <PipelineLayout.hpp>

#include "DescriptorPools.hpp"
#include "EntSpecLightShader.hpp"
#include "IShader.hpp"
#include "IShaderDescSet.hpp"
#include "IShaderPipeline.hpp"
#include "LightShader.hpp"
#include "entity.hpp"
#include "pch.h"

class EntityPipeline : public IShaderPipeline
{
    const GraphicCore::VulkanDevice&             m_Context;
    vk::Pipeline                                 graphicsPipeline;
    VulkanGame::Ref<GraphicCore::PipelineLayout> pipelineLayout;

    GraphicCore::VertexLayout vertex_layout {{GraphicCore::VertexLayout::Component::VERTEX_COMPONENT_POSITION,
                                              GraphicCore::VertexLayout::Component::VERTEX_COMPONENT_TEXT_COORD,
                                              GraphicCore::VertexLayout::Component::VERTEX_COMPONENT_NORMAL}};

   public:
    explicit EntityPipeline (const GraphicCore::VulkanDevice& Context);

    void CreateGraphicsPipeline (std::vector<std::string>&& shaderPatchName, const vk::Extent2D& swapChainWindowSize,
                                 const vk::RenderPass& renderPass) override;
    void CreatePipelineLayout (const IShaderDescSet& descSet) override;
    void Bind (const IShaderDescSet& descSet, const vk::CommandBuffer& cmdBufer,
               const size_t& frameIndex) const override;
    void Destroy( ) override;
};
#endif  // ENTITY_PIPELINE_HPP
