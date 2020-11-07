#include "EntityShader2.hpp"

EntityShader2::EntityShader2 (const GraphicCore::VulkanDevice& Device, const vk::RenderPass& renderPass,
                              const vk::Extent2D& swapChainWindowSize)
    : m_Device (Device)
    , m_RenderPass (renderPass)
    , m_SwapChainWindowSize (swapChainWindowSize)
{
}

void EntityShader2::init (const uint32_t& frames)
{
    createDescriptorSetLayout( );
    createDescriptorPool (frames);
    createDescriptorSets (frames);
    createGraphicsPipeline (getDescSetLayout( ));

    uniform.create (m_Device, frames, sizeof (UniformBufferObject));
    uniform2.create (m_Device, frames, sizeof (lightUniformBufferObject));
}

void EntityShader2::bind (const vk::CommandBuffer& cmdBufer, const size_t& frameIndex)
{
    cmdBufer.bindDescriptorSets (vk::PipelineBindPoint::eGraphics, pipelineLayout.getVkPipelineLayout( ), 0, 1,
                                 &getDescriptorSets( ).getDescriptorSetByIndex (frameIndex), 0, nullptr);

    cmdBufer.bindPipeline (vk::PipelineBindPoint::eGraphics, graphicsPipeline);
}

void EntityShader2::createGraphicsPipeline (const GraphicCore::DescriptorSetLayoutBinding& descSetLayout)
{
    pipelineLayout.create (m_Device.getVkDevice( ), descSetLayout);

    GraphicCore::Pipeline builder {m_Device.getVkDevice( ), pipelineLayout.getVkPipelineLayout( ), m_RenderPass};
    builder.getPipelineViewport( ).setViewportAndScissor (m_SwapChainWindowSize);
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

void EntityShader2::createDescriptorSets (const uint32_t& descSetsCount)
{
    descriptorSets.create (m_Device.getVkDevice( ), descSetsCount, descSetLayout);
}

void EntityShader2::createDescriptorPool (const uint32_t& poolSizeDescSetsCount)
{
    descriptorPool.addDescriptorPoolSize (vk::DescriptorType::eUniformBuffer, poolSizeDescSetsCount);
    descriptorPool.addDescriptorPoolSize (vk::DescriptorType::eCombinedImageSampler, poolSizeDescSetsCount);
    descriptorPool.addDescriptorPoolSize (vk::DescriptorType::eUniformBuffer, poolSizeDescSetsCount);

    descriptorPool.create (m_Device.getVkDevice( ), poolSizeDescSetsCount);
}

void EntityShader2::createDescriptorSetLayout( )
{
    descSetLayout.addDescriptorSetLayoutBinding (vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eVertex);
    descSetLayout.addDescriptorSetLayoutBinding (vk::DescriptorType::eCombinedImageSampler,
                                                 vk::ShaderStageFlagBits::eFragment);
    descSetLayout.addDescriptorSetLayoutBinding (vk::DescriptorType::eUniformBuffer,
                                                 vk::ShaderStageFlagBits::eFragment);

    std::vector<vk::DescriptorType> typesUsed {vk::DescriptorType::eUniformBuffer,
                                               vk::DescriptorType::eCombinedImageSampler,
                                               vk::DescriptorType::eUniformBuffer};

    descSetLayout.create (m_Device.getVkDevice( ), typesUsed);
}

void EntityShader2::destroy( )
{
    descSetLayout.destroy (m_Device.getVkDevice( ));

    m_Device.getVkDevice( ).destroyCommandPool (m_Device.getCommandPool( ));
}

const GraphicCore::DescriptorSetLayoutBinding& EntityShader2::getDescSetLayout( ) const { return descSetLayout; }
const GraphicCore::DescriptorSets&             EntityShader2::getDescriptorSets( ) const { return descriptorSets; }

void EntityShader2::updateWriteDescriptors( )
{
    descriptorSets.update( );
    descriptorSets.clearWriteDescSets( );
}
