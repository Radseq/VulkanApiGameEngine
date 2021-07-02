#include "pch.h"
#include "EntityPipeline.hpp"

EntityPipeline::EntityPipeline (const GraphicCore::VulkanDevice& Context)
    : m_Context (Context)
{
}

void EntityPipeline::CreateGraphicsPipeline (std::vector<std::string>&& shaderPatchName,
                                             const vk::Extent2D& swapChainWindowSize, const vk::RenderPass& renderPass)
{
    GraphicCore::Pipeline builder {m_Context.getVkDevice( ), pipelineLayout->getVkPipelineLayout( ), renderPass};
    builder.getPipelineViewport( ).setViewportAndScissor (swapChainWindowSize);
    builder.getVertexInputState( ).appendVertexLayout (vertex_layout);
    builder.getPipelineRasterization( ).getRasterizationState( ).cullMode = vk::CullModeFlagBits::eBack;
    builder.getPipelineRasterization( ).getRasterizationState( ).frontFace =
        vk::FrontFace::eCounterClockwise;  // eClockwise;
    builder.getPipelineColorBlend( ).logicOp = vk::LogicOp::eCopy;
    // builder.getMultisampleState( ).rasterizationSamples                    = msaaSamples;
    builder.getPipelineDepthStencil( ).getDepthStencil( ).depthCompareOp   = vk::CompareOp::eLess;
    builder.getPipelineDepthStencil( ).getDepthStencil( ).depthTestEnable  = VK_TRUE;
    builder.getPipelineDepthStencil( ).getDepthStencil( ).depthWriteEnable = VK_TRUE;

    vk::PipelineColorBlendAttachmentState colorBlendAttachment = { };
    colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                                          vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
    colorBlendAttachment.blendEnable = VK_FALSE;
    builder.getPipelineColorBlend( ).getBlendAttachmentStates( ).push_back (colorBlendAttachment);

    builder.loadShader (shaderPatchName [0], vk::ShaderStageFlagBits::eVertex);
    builder.loadShader (shaderPatchName [1], vk::ShaderStageFlagBits::eFragment);

    graphicsPipeline = builder.create( );
}

void EntityPipeline::CreatePipelineLayout (const IShaderDescSet& descSet)
{
    pipelineLayout = VulkanGame::CreateRef<GraphicCore::PipelineLayout> (m_Context);
    pipelineLayout->create (descSet.GetDescSetLayout( ));
}

void EntityPipeline::Bind (const IShaderDescSet& descSet, const vk::CommandBuffer& cmdBufer,
                           const size_t& frameIndex) const
{
    cmdBufer.bindDescriptorSets (vk::PipelineBindPoint::eGraphics, pipelineLayout->getVkPipelineLayout( ), 0, 1,
                                 &descSet.GetDescSets( )->getDescriptorSetByIndex (frameIndex), 0, nullptr);

    cmdBufer.bindPipeline (vk::PipelineBindPoint::eGraphics, graphicsPipeline);
}

void EntityPipeline::Destroy( )
{
    m_Context.getVkDevice( ).destroyPipeline (graphicsPipeline);
    m_Context.getVkDevice( ).destroyPipelineLayout (pipelineLayout->getVkPipelineLayout( ));
}
