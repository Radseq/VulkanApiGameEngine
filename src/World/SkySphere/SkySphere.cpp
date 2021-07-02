#include "SkySphere.hpp"

SkySphere::SkySphere (const GraphicCore::VulkanDevice& Context, const Camera& camera)
    : context (Context)
    , _camera (camera)
{
}

void SkySphere::destroy( )
{
    skySphereModel->destroy( );
    skySphereTexture->destroy( );
    skySphereVertex.destroy (context);
    descSetLayout.destroy (context.getVkDevice( ));
}

void SkySphere::loadAssets( )
{
    AssimpModelLoader assimpModelLoader { };
    assimpModelLoader.loadFromFile (
        skySphereModel, ResourcePatch::GetInstance( )->GetPatch ("DataPatch") + "/models/skysphere/geosphere.obj",
        10.F);

    std::string texFormatSuffix;
    vk::Format  texFormat;

    const auto& deviceFeatures = context.getDevice( ).GetPhysicalDeviceFeatures( );

    // Get supported compressed texture format
    if (deviceFeatures.textureCompressionBC)
    {
        texFormatSuffix = "_bc3_unorm";
        texFormat       = vk::Format::eBc3UnormBlock;
    }
    else if (deviceFeatures.textureCompressionASTC_LDR)
    {
        texFormatSuffix = "_astc_8x8_unorm";
        texFormat       = vk::Format::eAstc8x8UnormBlock;
    }
    else if (deviceFeatures.textureCompressionETC2)
    {
        texFormatSuffix = "_etc2_unorm";
        texFormat       = vk::Format::eEtc2R8G8B8A8UnormBlock;
    }
    else
    {
        throw std::runtime_error ("Device does not support any compressed texture format!");
    }

    VulkanGame::Ref<Texture2D> texture;
    KtxTextureLoader           textureLoader;

    auto textureContainer = textureLoader.LoadFile (ResourcePatch::GetInstance( )->GetPatch ("DataPatch") +
                                                    "/textures/skysphere/skysphere" + texFormatSuffix + ".ktx");
    // textureContainer.image.mipLevels = 1;

    skySphereTexture = texture->CreateImage (context, textureContainer, texFormat);
    skySphereTexture->setupDescriptorImageInfo (vk::ImageLayout::eShaderReadOnlyOptimal);
}

// Prepare and initialize uniform buffer containing shader uniforms
void SkySphere::prepareUniformBuffers (const glm::mat4& perspective)
{
    // todo set uboVS

    bufferManager.createUniformBuffer (skySphereVertex, uboVS);

    updateUniformBuffers (perspective);
}

void SkySphere::updateUniformBuffers (const glm::mat4& perspective)
{
    // uboVS.mvp = perspective * glm::mat4 (glm::mat3 (Math::createViewMatrix (_camera)));

    // todo fix me
    glm::mat4 m4 (1.0f);  // identity matrix
    m4 [0] = {-0.909567, -0.129053, 0.350605, 0.350537};

    m4 [1] = {0.000000, 1.694201, 0.207952, 0.207912};

    m4 [2] = {0.349150, -0.336195, 0.913358, 0.913180};

    m4 [3] = {0.000000, 0.000000, -0.100020, 0.000000};

    uboVS.mvp = m4;

    // std::cout << glm::to_string (uboVS.mvp) << std::endl;
    skySphereVertex.copy (uboVS);
}

void SkySphere::updateDrawCommandBuffer (const vk::CommandBuffer& cmdBuffer)
{
    cmdBuffer.bindPipeline (vk::PipelineBindPoint::eGraphics, skyspherePipeline);
    cmdBuffer.bindDescriptorSets (vk::PipelineBindPoint::eGraphics, skyspherePipelineLayout, 0, skySphereDescriptorSet,
                                  { });
    cmdBuffer.bindVertexBuffers (0, skySphereModel->vertexCoreBuffer.getBuffer( ), {0});
    cmdBuffer.bindIndexBuffer (skySphereModel->indexCoreBuffer.getBuffer( ), 0, vk::IndexType::eUint32);
    cmdBuffer.drawIndexed (skySphereModel->indexCount, 1, 0, 0, 0);
}

void SkySphere::descriptorDestroy( )
{
    descSetLayout.destroy (context.getVkDevice( ));
    // context.getVkDevice().destroyDescriptorPool(descriptorPool);
}

void SkySphere::setupDescriptorSets (GraphicCore::DescriptorPool& descPool)
{
    skySphereDescriptorSet = context.getVkDevice( ).allocateDescriptorSets (
        {descPool.getDescriptorPool( ), 1, &descSetLayout.getDescriptorSetLayout( )}) [0];

    std::vector<vk::WriteDescriptorSet> writeDescriptorSets {
        {skySphereDescriptorSet, 0, 0, 1, vk::DescriptorType::eUniformBuffer, nullptr,
         &skySphereVertex.getDescriptorInfo( )},
        {skySphereDescriptorSet, 1, 0, 1, vk::DescriptorType::eCombinedImageSampler,
         &skySphereTexture->getDescImageInfo( )},
    };

    context.getVkDevice( ).updateDescriptorSets (writeDescriptorSets, nullptr);
}

void SkySphere::createPipelines (vk::RenderPass const& renderPass)
{
    createDescriptorSetLayouts( );

    // Terrain tessellation pipeline
    GraphicCore::Pipeline builder {context.getVkDevice( ), skyspherePipelineLayout, renderPass};
    builder.getPipelineDynamic( ).getDynamicStateEnables( ).push_back (vk::DynamicState::eViewport);
    builder.getPipelineDynamic( ).getDynamicStateEnables( ).push_back (vk::DynamicState::eLineWidth);
    builder.getPipelineDynamic( ).getDynamicStateEnables( ).push_back (vk::DynamicState::eScissor);
    builder.getVertexInputState( ).appendVertexLayout (vertexLayout);
    // Skysphere pipeline
    builder.getPipelineRasterization( ).getRasterizationState( ).polygonMode = vk::PolygonMode::eFill;
    // Revert to triangle list topology
    builder.getPipelineInputAssembly( ).getAssembly( ).topology = vk::PrimitiveTopology::eTriangleList;
    // Reset tessellation state
    builder.getPipelineCreateInfo( ).pTessellationState = nullptr;
    // Don't write to depth buffer
    builder.getPipelineDepthStencil( ).getDepthStencil( ).depthWriteEnable = VK_FALSE;
    vk::PipelineDepthStencilStateCreateInfo depthStencil;
    depthStencil.depthTestEnable  = VK_TRUE;
    depthStencil.depthCompareOp   = vk::CompareOp::eLessOrEqual;
    depthStencil.depthWriteEnable = VK_FALSE;
    depthStencil.back.compareOp   = vk::CompareOp::eAlways;
    depthStencil.front.compareOp  = vk::CompareOp::eNever;

    vk::PipelineColorBlendAttachmentState colorBlendAttahment;
    // colorBlendAttahment.blendEnable = VK_TRUE;
    colorBlendAttahment.colorWriteMask = vk::ColorComponentFlagBits::eA | vk::ColorComponentFlagBits::eB |
                                         vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eR;
    builder.getPipelineColorBlend( ).getBlendAttachmentStates( ).push_back (colorBlendAttahment);

    builder.getPipelineDepthStencil( ).getDepthStencil( ) = depthStencil;
    builder.getLayout( )                                  = skyspherePipelineLayout;

    builder.loadShader (ResourcePatch::GetInstance( )->GetPatch ("DataPatch") + "/shaders/skysphere/vert.spv",
                        vk::ShaderStageFlagBits::eVertex);
    builder.loadShader (ResourcePatch::GetInstance( )->GetPatch ("DataPatch") + "/shaders/skysphere/frag.spv",
                        vk::ShaderStageFlagBits::eFragment);

    skyspherePipeline = builder.create( );
}

void SkySphere::createDescriptorSetLayouts( )
{
    descSetLayout.addDescriptorSetLayoutBinding (vk::DescriptorType::eUniformBuffer,
                                                 {vk::ShaderStageFlagBits::eVertex});
    descSetLayout.addDescriptorSetLayoutBinding (vk::DescriptorType::eCombinedImageSampler,
                                                 {vk::ShaderStageFlagBits::eFragment});
    /*
    std::vector<vk::DescriptorType> typesUsed {{vk::DescriptorType::eUniformBuffer},
                                               {vk::DescriptorType::eCombinedImageSampler}};*/

    descSetLayout.create (context.getVkDevice( ) /*, typesUsed*/);

    skyspherePipelineLayout = context.getVkDevice( ).createPipelineLayout (
        vk::PipelineLayoutCreateInfo {{ }, 1, &descSetLayout.getDescriptorSetLayout( )});
}