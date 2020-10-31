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
    GameCore::VertexLayout vertex_layout {{GameCore::VertexLayout::Component::VERTEX_COMPONENT_POSITION,
                                           GameCore::VertexLayout::Component::VERTEX_COMPONENT_TEXT_COORD,
                                           GameCore::VertexLayout::Component::VERTEX_COMPONENT_NORMAL}};

    vk::Pipeline graphicsPipeline;

    GameCore::PipelineLayout      pipelineLayout { };
    const GameCore::VulkanDevice& context;
    const GameCore::SwapChain&    swapChain;

   public:
    EntityPipeline (const GameCore::VulkanDevice& Context, const GameCore::SwapChain& SwapChain);

    void createGraphicsPipeline (const vk::RenderPass&                       renderPass,
                                 const GameCore::DescriptorSetLayoutBinding& descSetLayout);

    const GameCore::PipelineLayout& getPipelineLayout( );
    const vk::Pipeline&             getVkPipeline( );
};

#endif  // ENTITY_PIPELINE_HPP
