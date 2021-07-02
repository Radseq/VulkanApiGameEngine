#ifndef SKYSPHERE_HPP
#define SKYSPHERE_HPP

#include <Descriptor\DescriptorPool.hpp>
#include <Descriptor\DescriptorSetLayout.hpp>
#include <Image\Image.hpp>
#include <glm\gtx\string_cast.hpp>

#include "../../FileLoader/AssimpModelLoader.hpp"
#include "../../FileLoader/TinyObjModelLoader.hpp"
#include "../../Model/Model.hpp"
#include "../../Textures/Texture2D.hpp"
#include "../../entities/camera.hpp"
#include "Buffer/CoreBuffeManager.hpp"
#include "Buffer/CoreBuffer.hpp"
#include "Device/VulkanDevice.hpp"
#include "Pipeline/Pipeline.hpp"

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

    VulkanGame::Ref<GraphicCore::Image> skySphereTexture;

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
