#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include <Buffer\CoreBuffeManager.hpp>
#include <Descriptor\DescriptorPool.hpp>
#include <Descriptor\DescriptorSetLayout.hpp>
#include <Image\Image.hpp>

#include "../../Textures/Texture2DArray.hpp"
#include "Buffer/CoreBuffer.hpp"
#include "Device/VulkanDevice.hpp"
#include "HeightMap.hpp"
#include "Pipeline/Pipeline.hpp"
#include "Util/Frustum.hpp"
#include "../../entities/camera.hpp"

class Terrain {
    const GameCore::VulkanDevice& context;

    struct {
        glm::mat4 projection;
        glm::mat4 modelview;
        glm::vec4 lightPos = glm::vec4 (-48.0F, -40.0F, 46.0F, 0.0F);
        glm::vec4 frustumPlanes [6];
        float     displacementFactor = 32.0F;
        float     tessellationFactor = 0.75F;
        glm::vec2 viewportDim;
        // Desired size of tessellated quad patch edge
        float tessellatedEdgeSize = 20.0F;
    } uboTess;

    // Terrain Pipeline statistics
    // GameCore::CoreBuffer queryResult;
    // vk::QueryPool queryPool;
    // std::array<uint64_t, 2> pipelineStats{0, 0};

    // View frustum passed to tessellation control shader for culling
    GameCore::Frustum frustum { };

    bool tessellation = true;
    bool wireframe    = false;

    GameCore::CoreBuffer terrainTessellation;
    GameCore::CoreBuffer terrainVertexBuffer;
    GameCore::CoreBuffer terrainIndexBuffer;

    uint32_t terrainIndices {0};

    Texture2DArray  terrainArrayTexture {context};
    GameCore::Image terrainArrayTextureImage;
    GameCore::Image heightMapTextureImage;

    Camera& _camera;

    GameCore::CoreBufferManager bufferManager {context};

    GameCore::VertexLayout vertexLayout {{
        GameCore::VertexLayout::Component::VERTEX_COMPONENT_POSITION,
        GameCore::VertexLayout::Component::VERTEX_COMPONENT_NORMAL,
        GameCore::VertexLayout::Component::VERTEX_COMPONENT_UV,
    }};

    vk::Pipeline       terrainPipeline;
    vk::Pipeline       wireframePipeline;
    vk::PipelineLayout terrainPipelineLayout;

    vk::DescriptorSet terrainDescriptorSet;

    GameCore::DescriptorSetLayoutBinding descSetLayout { };

   public:
    Terrain (const GameCore::VulkanDevice& Context, Camera& camera);

    void destroy( );
    void descriptorDestroy( );

    void  loadAssets( );
    void  prepareUniformBuffers (const vk::Extent2D& windowSize, const glm::mat4& perspective);
    void  updateUniformBuffers (const vk::Extent2D& windowSize, const glm::mat4& perspective);
    void  updateDrawCommandBuffer (const vk::CommandBuffer& cmdBuffer);
    bool& useWireframe( );
    bool& useTessellation( );

    // Setup pool and buffer for storing pipeline statistics results
    void setupQueryResultBuffer( );

    // Retrieves the results of the pipeline statistics query submitted to the command buffer
    void getQueryResults( );
    void updateCommandBufferPreDraw (const vk::CommandBuffer& cmdBuffer);

    // const std::array<uint64_t, 2> getPipelineStats();

    // Generate a terrain quad patch for feeding to the tessellation control shader
    void generateTerrain( );

    float& GetTessellationFactor( );  // cant const cuse ui uses oryginal reference to modify

    void setupDescriptorSets (GameCore::DescriptorPool& descPool);
    void createDescriptorSetLayouts( );
    void createPipeline (const vk::RenderPass& renderPass);

    const vk::Pipeline& getPipelineTerrain( ) const;
    const vk::Pipeline& getPipelineWireframe( ) const;

    const vk::PipelineLayout& getTerrainPipelineLayout( ) const;
};

#endif  // TERRAIN_HPP
