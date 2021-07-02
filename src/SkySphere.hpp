#ifndef SKYSPHERE_HPP
#define SKYSPHERE_HPP

#include <CoreBuffeManager.hpp>
#include <CoreBuffer.hpp>
#include <CoreImage.hpp>
#include <DescriptorPool.hpp>
#include <DescriptorSetLayout.hpp>
#include <ImageSampler.hpp>
#include <Pipeline.hpp>
#include <VulkanDevice.hpp>

#include "AssimpModelLoader.hpp"
#include "Model.hpp"
#include "ResourcePatch.hpp"
#include "Texture2D.hpp"
#include "TinyObjModelLoader.hpp"
#include "camera.hpp"

class SkySphere
{
    GraphicCore::VertexLayout vertexLayout {{
        GraphicCore::VertexLayout::Component::VERTEX_COMPONENT_POSITION,
        GraphicCore::VertexLayout::Component::VERTEX_COMPONENT_NORMAL,
        GraphicCore::VertexLayout::Component::VERTEX_COMPONENT_UV,
    }};

    // Skysphere vertex shader stage
    struct
    {
        glm::mat4 mvp;
    } uboVS;

    const GraphicCore::VulkanDevice& context;
    GraphicCore::CoreBuffer          skySphereVertex;

    VulkanGame::Ref<GraphicCore::CoreImage> skySphereTexture;

    vk::DescriptorSet skySphereDescriptorSet;

    vk::PipelineLayout      skyspherePipelineLayout;
    vk::Pipeline            skyspherePipeline;
    vk::DescriptorSetLayout skysphereDescriptorSetLayout;

    GraphicCore::CoreBufferManager bufferManager {context};
    const Camera&                  _camera;

    // const SkySpherePipeline& skySpherePipeline;

    // TinyObjModelLoader				skySphereModel{ context };

    Model* skySphereModel = new Model (context, vertexLayout);

    GraphicCore::DescriptorSetLayoutBinding descSetLayout { };

    GraphicCore::ImageSampler skySphereTextureImageSampler {context.getVkDevice( )};

    vk::DescriptorImageInfo skySphereTextureDescImageInfo { };

   public:
    SkySphere (const GraphicCore::VulkanDevice& Context, const Camera& camera);

    void destroy( );
    void loadAssets( );
    void prepareUniformBuffers (const glm::mat4& perspective);
    void updateUniformBuffers (const glm::mat4& perspective);
    void updateDrawCommandBuffer (const vk::CommandBuffer& cmdBuffer);
    void descriptorDestroy( );
    void setupDescriptorSets (GraphicCore::DescriptorPool& descPool);
    void createPipelines (vk::RenderPass const& renderPass);
    void createDescriptorSetLayouts( );
};

#endif  // SKYSPHERE_HPP
