#include "EntityShader.hpp"

EntityShader::EntityShader (const GraphicCore::VulkanDevice& Device, const GraphicCore::SwapChain& SwapChain,
                            const EntityDescriptor& EntityDescriptor)
    : device (Device)
    , swapChain (SwapChain)
    , descriptor (EntityDescriptor)
{
}

void EntityShader::init (const vk::RenderPass& renderPass)
{
    pipeline.createGraphicsPipeline (renderPass, descriptor.getDescSetLayout( ));
}

void EntityShader::bind (const vk::CommandBuffer& cmdBufer, const size_t& frameIndex)
{
    cmdBufer.bindDescriptorSets (vk::PipelineBindPoint::eGraphics, pipeline.getPipelineLayout( ).getVkPipelineLayout( ),
                                 0, 1, &descriptor.getDescriptorSets( ).getDescriptorSetByIndex (frameIndex), 0,
                                 nullptr);

    cmdBufer.bindPipeline (vk::PipelineBindPoint::eGraphics, pipeline.getVkPipeline( ));
}