#ifndef ENTITY_PIPELINE_HPP
#define ENTITY_PIPELINE_HPP

#include <Device\VulkanDevice.hpp>
#include <Framebuffer\SwapChain.hpp>
#include <Model\VertexLayout.hpp>
#include <Pipeline/Pipeline.hpp>
#include <Pipeline\PipelineLayout.hpp>

class EntityPipeline
{
    // must be in correct order of model vertexCoreBuffer loaded(from file)
    GraphicCore::VertexLayout vertex_layout {{GraphicCore::VertexLayout::Component::VERTEX_COMPONENT_POSITION,
                                           GraphicCore::VertexLayout::Component::VERTEX_COMPONENT_TEXT_COORD,
                                           GraphicCore::VertexLayout::Component::VERTEX_COMPONENT_NORMAL}};

    vk::Pipeline graphicsPipeline;

    GraphicCore::PipelineLayout      pipelineLayout { };
    const GraphicCore::VulkanDevice& context;
    const GraphicCore::SwapChain&    swapChain;

   public:
    EntityPipeline (const GraphicCore::VulkanDevice& Context, const GraphicCore::SwapChain& SwapChain);

    void createGraphicsPipeline (const vk::RenderPass&                       renderPass,
                                 const GraphicCore::DescriptorSetLayoutBinding& descSetLayout);

    const GraphicCore::PipelineLayout& getPipelineLayout( );
    const vk::Pipeline&             getVkPipeline( );
};

#endif  // ENTITY_PIPELINE_HPP
