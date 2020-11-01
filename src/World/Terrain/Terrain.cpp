#include "Terrain.hpp"

#include "../../FileLoader/KtxTextureLoader.hpp"
#include "../../Textures/Texture2D.hpp"

Terrain::Terrain (const GameCore::VulkanDevice& Context, Camera& camera)
    : context (Context)
    , _camera (camera) { }

void Terrain::destroy( ) {
    terrainTessellation.destroy (context);
    terrainArrayTextureImage.destroy( );
    heightMapTextureImage.destroy( );

    terrainTessellation.destroy (context);
    terrainVertexBuffer.destroy (context);
    terrainIndexBuffer.destroy (context);

    // queryResult.destroy();

    context.getVkDevice( ).destroyPipeline (wireframePipeline);
    context.getVkDevice( ).destroyPipeline (terrainPipeline);
    context.getVkDevice( ).destroyPipelineLayout (terrainPipelineLayout);
}

void Terrain::descriptorDestroy( ) {
    // context.getVkDevice().destroyDescriptorPool(descriptorPool);
    descSetLayout.destroy (context.getVkDevice( ));
}

void Terrain::loadAssets( ) {
    // Textures
    std::string texFormatSuffix;
    vk::Format  texFormat;
    // Get supported compressed texture format

    const auto& deviceFeatures = context.getDevice( ).GetPhysicalDeviceFeatures( );

    if (deviceFeatures.textureCompressionBC) {
        texFormatSuffix = "_bc3_unorm";
        texFormat       = vk::Format::eBc3UnormBlock;
    } else if (deviceFeatures.textureCompressionASTC_LDR) {
        texFormatSuffix = "_astc_8x8_unorm";
        texFormat       = vk::Format::eAstc8x8UnormBlock;
    } else if (deviceFeatures.textureCompressionETC2) {
        texFormatSuffix = "_etc2_unorm";
        texFormat       = vk::Format::eEtc2R8G8B8A8UnormBlock;
    } else {
        throw std::runtime_error ("Device does not support any compressed texture format!");
    }

    // Terrain textures are stored in a texture array with layers corresponding to terrain height
    terrainArrayTextureImage.layerCount = 1;
    terrainArrayTexture.LoadTexture (terrainArrayTextureImage,
                                     getAssetPath( ) + "textures/terrain_texturearray" + texFormatSuffix + ".ktx",
                                     texFormat);

    // Height data is stored in a one-channel texture
    Texture2D        texture;
    KtxTextureLoader textureLoader;

    auto textureContainer            = textureLoader.LoadFile (getAssetPath( ) + "textures/terrain_heightmap_r16.ktx");
    textureContainer.mipLevels = 1;

    vk::Format heightMapTexFormat {vk::Format::eR16Unorm};
    texture.CreateImage (heightMapTextureImage, context, textureContainer, heightMapTexFormat);

    // Setup a mirroring sampler for the height map
    context.getVkDevice( ).destroySampler (heightMapTextureImage.sampler);
    vk::SamplerCreateInfo samplerInfo;
    samplerInfo.minFilter = samplerInfo.magFilter = vk::Filter::eLinear;
    samplerInfo.mipmapMode                        = vk::SamplerMipmapMode::eLinear;
    samplerInfo.maxLod                            = static_cast<float> (heightMapTextureImage.mipLevels);
    samplerInfo.borderColor                       = vk::BorderColor::eFloatOpaqueWhite;
    samplerInfo.addressModeU                      = vk::SamplerAddressMode::eMirroredRepeat;
    samplerInfo.addressModeV                      = samplerInfo.addressModeU;
    samplerInfo.addressModeW                      = samplerInfo.addressModeU;
    samplerInfo.maxAnisotropy                     = 1.0F;

    heightMapTextureImage.sampler = context.getVkDevice( ).createSampler (samplerInfo);

    heightMapTextureImage.setupDescriptorImageInfo (vk::ImageLayout::eShaderReadOnlyOptimal);

    // Setup a repeating sampler for the terrain texture layers
    context.getVkDevice( ).destroySampler (terrainArrayTextureImage.sampler);

    samplerInfo.maxLod       = static_cast<float> (terrainArrayTextureImage.mipLevels);
    samplerInfo.addressModeU = vk::SamplerAddressMode::eRepeat;
    samplerInfo.addressModeV = samplerInfo.addressModeU;
    samplerInfo.addressModeW = samplerInfo.addressModeU;

    if (context.getDevice( ).GetPhysicalDeviceFeatures( ).samplerAnisotropy) {
        samplerInfo.maxAnisotropy    = 4.0F;
        samplerInfo.anisotropyEnable = VK_TRUE;
    }

    terrainArrayTextureImage.sampler = context.getVkDevice( ).createSampler (samplerInfo);
    terrainArrayTextureImage.setupDescriptorImageInfo (vk::ImageLayout::eShaderReadOnlyOptimal);
}

// Prepare and initialize uniform buffer containing shader uniforms
void Terrain::prepareUniformBuffers (const vk::Extent2D& windowSize, const glm::mat4& perspective)
{
    bufferManager.createUniformBuffer (terrainTessellation, uboTess);

    updateUniformBuffers (windowSize, perspective);
}

void Terrain::updateUniformBuffers (const vk::Extent2D& windowSize, const glm::mat4& perspective)
{
    uboTess.projection  = perspective;
    uboTess.modelview   = Math::createViewMatrix (_camera);
    uboTess.lightPos.y  = -0.5F - uboTess.displacementFactor;  // todo: Not uesed yet
    uboTess.viewportDim = glm::vec2 (static_cast<float> (windowSize.width), static_cast<float> (windowSize.height));

    frustum.update (uboTess.projection * uboTess.modelview);
    memcpy (uboTess.frustumPlanes, frustum.planes.data( ), sizeof (glm::vec4) * 6);

    float savedFactor = uboTess.tessellationFactor;
    if (!tessellation)
    {
        // Setting this to zero sets all tessellation factors to 1.0 in the shader
        uboTess.tessellationFactor = 0.0F;
    }

    terrainTessellation.copy (uboTess);

    if (!tessellation) { uboTess.tessellationFactor = savedFactor; }
}

void Terrain::updateDrawCommandBuffer (const vk::CommandBuffer& cmdBuffer) {
    // Terrrain

    // Begin pipeline statistics query
    // if (context.getDevice().GetPhysicalDeviceFeatures().pipelineStatisticsQuery)
    //{
    //	cmdBuffer.beginQuery(queryPool, 0, vk::QueryControlFlagBits::ePrecise);
    //}

    // Render
    cmdBuffer.bindPipeline (vk::PipelineBindPoint::eGraphics, wireframe ? wireframePipeline : terrainPipeline);
    cmdBuffer.bindDescriptorSets (vk::PipelineBindPoint::eGraphics, terrainPipelineLayout, 0, terrainDescriptorSet,
                                  { });
    cmdBuffer.bindVertexBuffers (0, terrainVertexBuffer.getBuffer( ), {0});
    cmdBuffer.bindIndexBuffer (terrainIndexBuffer.getBuffer( ), 0, vk::IndexType::eUint32);
    cmdBuffer.drawIndexed (terrainIndices, 1, 0, 0, 0);
    // End pipeline statistics query
    // if (context.getDevice().GetPhysicalDeviceFeatures().pipelineStatisticsQuery)
    //{
    //	cmdBuffer.endQuery(queryPool, 0);
    //}
}

/// <summary>
/// Setup pool and buffer for storing pipeline statistics results
/// use after prepare frame and before cmd render
/// </summary>
void Terrain::setupQueryResultBuffer( ) {
    /*
    const uint32_t bufSize = 2 * sizeof(uint64_t);
    // Results are saved in a host visible buffer for easy access by the application
    queryResult = *bufferManager.createBuffer(
            vk::BufferUsageFlagBits::eUniformBuffer | vk::BufferUsageFlagBits::eTransferDst,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
            bufSize);

    // Create query pool
    const auto pipelineStatistics =
            vk::QueryPipelineStatisticFlagBits::eVertexShaderInvocations | vk::QueryPipelineStatisticFlagBits::
            eTessellationEvaluationShaderInvocations;

    queryPool = context.getVkDevice().createQueryPool({{}, vk::QueryType::ePipelineStatistics, 2, pipelineStatistics});
    */
}

// Retrieves the results of the pipeline statistics query submitted to the command buffer
void Terrain::getQueryResults( ) {
    /*
    if (!context.getDevice().GetPhysicalDeviceFeatures().pipelineStatisticsQuery)
    {
            return;
    }

    // We use vkGetQueryResults to copy the results into a host visible buffer
    const vk::Result result = context.getVkDevice().getQueryPoolResults<uint64_t>(queryPool,
                                                                                                                              0,
                                                                                                                              1,
                                                                                                                              pipelineStats,
                                                                                                                              sizeof(uint64_t),
                                                                                                                              vk::QueryResultFlagBits::e64);
    if (result != vk::Result::eSuccess)
    {
            throw std::runtime_error("error in get query pool results");
    }*/
}

void Terrain::updateCommandBufferPreDraw (const vk::CommandBuffer& cmdBuffer) {
    /*if (context.getDevice().GetPhysicalDeviceFeatures().pipelineStatisticsQuery)
    {
            cmdBuffer.resetQueryPool(queryPool, 0, 2);
    }*/
}

void Terrain::generateTerrain( ) {
    struct Vertex {
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec2 uv;
    };

    constexpr uint32_t PATCH_SIZE = 64;
    constexpr float    UV_SCALE   = 1.0F;
    constexpr float    wx         = 2.0F;
    constexpr float    wy         = 2.0F;

    constexpr uint32_t              vertexCount = PATCH_SIZE * PATCH_SIZE;
    std::array<Vertex, vertexCount> vertices;

    for (auto x = 0; x < PATCH_SIZE; ++x) {
        for (auto y = 0; y < PATCH_SIZE; ++y) {
            uint32_t index           = (x + y * PATCH_SIZE);
            vertices [index].pos [0] = x * wx + wx / 2.0F - static_cast<float> (PATCH_SIZE) * wx / 2.0F;
            vertices [index].pos [1] = 0.0F;
            vertices [index].pos [2] = y * wy + wy / 2.0F - static_cast<float> (PATCH_SIZE) * wy / 2.0F;
            vertices [index].uv =
                glm::vec2 (static_cast<float> (x) / PATCH_SIZE, static_cast<float> (y) / PATCH_SIZE) * UV_SCALE;
        }
    }

    HeightMap heightMap { };
    heightMap.loadHeightMap (getAssetPath( ) + "textures/terrain_heightmap_r16.ktx", PATCH_SIZE);

    // Calculate normals from height map using a sobel filter
    for (auto x = 0; x < PATCH_SIZE; x++) {
        for (auto y = 0; y < PATCH_SIZE; y++) {
            // Get height samples centered around current position
            // float heights [3][3];
            std::array<std::array<int, 3>, 3> heights;
            for (auto hx = -1; hx <= 1; hx++) {
                for (auto hy = -1; hy <= 1; hy++) { heights [hx + 1][hy + 1] = heightMap.getHeight (x + hx, y + hy); }
            }

            // Calculate the normal
            glm::vec3 normal;
            // Gx sobel filter
            normal.x = heights [0][0] - heights [2][0] + 2.0F * heights [0][1] - 2.0F * heights [2][1] +
                       heights [0][2] - heights [2][2];
            // Gy sobel filter
            normal.z = heights [0][0] + 2.0F * heights [1][0] + heights [2][0] - heights [0][2] -
                       2.0F * heights [1][2] - heights [2][2];
            // Calculate missing up component of the normal using the filtered x and y axis
            // The first value controls the bump strength
            normal.y = 0.25F * sqrt (1.0F - normal.x * normal.x - normal.z * normal.z);

            vertices [x + y * PATCH_SIZE].normal = glm::normalize (normal * glm::vec3 (2.0F, 1.0F, 2.0F));
        }
    }

    // Indices
    constexpr uint32_t w          = (PATCH_SIZE - 1);
    constexpr uint32_t indexCount = w * w * 4;

    std::array<uint32_t, indexCount> indices { };
    // uint32_t*                        indices = new uint32_t [indexCount];

    for (auto x = 0; x < w; x++) {
        for (auto y = 0; y < w; y++) {
            uint32_t index      = (x + y * w) * 4;
            indices [index]     = (x + y * PATCH_SIZE);
            indices [index + 1] = indices [index] + PATCH_SIZE;
            indices [index + 2] = indices [index + 1] + 1;
            indices [index + 3] = indices [index] + 1;
        }
    }

    terrainIndices = indexCount;

    bufferManager.stageToDeviceBuffer (terrainVertexBuffer, vk::BufferUsageFlagBits::eVertexBuffer, vertices);
    bufferManager.stageToDeviceBuffer (terrainIndexBuffer, vk::BufferUsageFlagBits::eIndexBuffer, indices);

    // delete[] indices;
}

void Terrain::setupDescriptorSets (GameCore::DescriptorPool& descPool) {
    terrainDescriptorSet = context.getVkDevice( ).allocateDescriptorSets (
        {descPool.getDescriptorPool( ), 1, &descSetLayout.getDescriptorSetLayout( )}) [0];

    std::vector<vk::WriteDescriptorSet> writeDescriptorSets = {
        {terrainDescriptorSet, 0, 0, 1, vk::DescriptorType::eUniformBuffer, nullptr,
         &terrainTessellation.getDescriptorInfo( )},
        {terrainDescriptorSet, 1, 0, 1, vk::DescriptorType::eCombinedImageSampler,
         &heightMapTextureImage.getDescImageInfo()},
        {terrainDescriptorSet, 2, 0, 1, vk::DescriptorType::eCombinedImageSampler,
         &terrainArrayTextureImage.getDescImageInfo()}};

    context.getVkDevice( ).updateDescriptorSets (writeDescriptorSets, nullptr);
}

// const std::array<uint64_t, 2> Terrain::getPipelineStats()
//{
//	return pipelineStats;
//}

void Terrain::createPipeline (const vk::RenderPass& renderPass) {
    createDescriptorSetLayouts( );
    // Terrain tessellation pipeline
    GameCore::Pipeline builder {context.getVkDevice( ), terrainPipelineLayout, renderPass};
    builder.getPipelineInputAssembly( ).getAssembly( ).topology = vk::PrimitiveTopology::ePatchList;
    // builder.getPipelineDynamic().getDynamicStateEnables().push_back(vk::DynamicState::eViewport);
    builder.getPipelineDynamic( ).getDynamicStateEnables( ).push_back (vk::DynamicState::eLineWidth);
    // builder.getPipelineDynamic().getDynamicStateEnables().push_back(vk::DynamicState::eScissor);
    builder.getVertexInputState( ).appendVertexLayout (vertexLayout);

    // We render the terrain as a grid of quad patches
    vk::PipelineTessellationStateCreateInfo tessellationState {{ }, 4};
    builder.loadShader (getAssetPath( ) + "shaders/terraintessellation/", "vert.spv", vk::ShaderStageFlagBits::eVertex);
    builder.loadShader (getAssetPath( ) + "shaders/terraintessellation/", "frag.spv",
                        vk::ShaderStageFlagBits::eFragment);
    builder.loadShader (getAssetPath( ) + "shaders/terraintessellation/", "tesc.spv",
                        vk::ShaderStageFlagBits::eTessellationControl);
    builder.loadShader (getAssetPath( ) + "shaders/terraintessellation/", "tese.spv",
                        vk::ShaderStageFlagBits::eTessellationEvaluation);

    builder.getPipelineCreateInfo( ).pTessellationState = &tessellationState;

    builder.getPipelineDepthStencil( ).getDepthStencil( ).depthTestEnable  = 1;
    builder.getPipelineDepthStencil( ).getDepthStencil( ).depthWriteEnable = 1;
    builder.getPipelineDepthStencil( ).getDepthStencil( ).depthCompareOp   = vk::CompareOp::eLessOrEqual;
    builder.getPipelineDepthStencil( ).getDepthStencil( ).back.compareOp   = vk::CompareOp::eAlways;

    builder.getPipelineColorBlend( ).attachmentCount = 1;

    vk::PipelineColorBlendAttachmentState colorBlend { };
    colorBlend.colorWriteMask = GameCore::vkHelper::fullColorWriteMask( );

    builder.getPipelineColorBlend( ).getBlendAttachmentStates( ).push_back (colorBlend);

    std::vector<vk::DynamicState> dynamicStateEnables = {vk::DynamicState::eViewport, vk::DynamicState::eScissor,
                                                         vk::DynamicState::eLineWidth};

    builder.getPipelineDynamic( ).getDynamicStateEnables( ) = dynamicStateEnables;

    terrainPipeline = builder.create( );

    // Terrain wireframe pipeline
    builder.getPipelineRasterization( ).getRasterizationState( ).polygonMode = vk::PolygonMode::eLine;
    wireframePipeline                                                        = builder.create( );
}

void Terrain::createDescriptorSetLayouts( ) {
    // Binding 0 : Shared Tessellation shader ubo
    descSetLayout.addDescriptorSetLayoutBinding (
        vk::DescriptorType::eUniformBuffer,
        vk::ShaderStageFlagBits::eTessellationControl | vk::ShaderStageFlagBits::eTessellationEvaluation);
    // Binding 1 : Height map
    descSetLayout.addDescriptorSetLayoutBinding (vk::DescriptorType::eCombinedImageSampler,
                                                 vk::ShaderStageFlagBits::eTessellationControl |
                                                     vk::ShaderStageFlagBits::eTessellationEvaluation |
                                                     vk::ShaderStageFlagBits::eFragment);
    // Binding 3 : Terrain texture array layers
    descSetLayout.addDescriptorSetLayoutBinding (vk::DescriptorType::eCombinedImageSampler,
                                                 vk::ShaderStageFlagBits::eFragment);

    std::vector<vk::DescriptorType> typesUsed {{vk::DescriptorType::eUniformBuffer},
                                                                    {vk::DescriptorType::eCombinedImageSampler},
                                                                    {vk::DescriptorType::eCombinedImageSampler}};

    descSetLayout.create (context.getVkDevice( ), typesUsed);

    terrainPipelineLayout = context.getVkDevice( ).createPipelineLayout (
        vk::PipelineLayoutCreateInfo {{ }, 1, &descSetLayout.getDescriptorSetLayout( )});
}

const vk::Pipeline&       Terrain::getPipelineTerrain( ) const { return terrainPipeline; }
const vk::Pipeline&       Terrain::getPipelineWireframe( ) const { return wireframePipeline; }
const vk::PipelineLayout& Terrain::getTerrainPipelineLayout( ) const { return terrainPipelineLayout; }
bool&                     Terrain::useTessellation( ) { return tessellation; }
float&                    Terrain::GetTessellationFactor( ) { return uboTess.tessellationFactor; }
bool&                     Terrain::useWireframe( ) { return wireframe; }
