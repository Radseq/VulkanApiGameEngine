#include "EntityPipeline.hpp"

EntityPipeline::EntityPipeline (const GraphicCore::VulkanDevice& Context, const GraphicCore::SwapChain& SwapChain)
    : context (Context)
    , swapChain (SwapChain)
{
    // msaaSamples = context.getDevice( ).getMaxUsableSampleCount( );
}

void EntityPipeline::createGraphicsPipeline (const vk::RenderPass&                       renderPass,
                                             const GraphicCore::DescriptorSetLayoutBinding& descSetLayout)
{
    pipelineLayout.create (context.getVkDevice( ), descSetLayout);

    GraphicCore::Pipeline builder {context.getVkDevice( ), pipelineLayout.getVkPipelineLayout( ), renderPass};
    builder.getPipelineViewport( ).setViewportAndScissor (swapChain.getSwapChainExtent( ));
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

    builder.loadShader (getFilePath( ) + "/../../data/shaders/static_shader/specular_lighting/", "vert.spv",
                        vk::ShaderStageFlagBits::eVertex);
    builder.loadShader (getFilePath( ) + "/../../data/shaders/static_shader/specular_lighting/", "frag.spv",
                        vk::ShaderStageFlagBits::eFragment);

    graphicsPipeline = builder.create( );
}

const GraphicCore::PipelineLayout& EntityPipeline::getPipelineLayout( ) { return pipelineLayout; }
const vk::Pipeline&             EntityPipeline::getVkPipeline( ) { return graphicsPipeline; }
