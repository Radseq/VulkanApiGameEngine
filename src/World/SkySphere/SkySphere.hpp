#ifndef SKYSPHERE_HPP
#define SKYSPHERE_HPP

#include <Descriptor\DescriptorPool.hpp>
#include <Descriptor\DescriptorSetLayout.hpp>
#include <Image\Image.hpp>

#include "../../FileLoader/AssimpModelLoader.hpp"
#include "../../FileLoader/TinyObjModelLoader.hpp"
#include "../../Textures/Texture2D.hpp"
#include "Buffer/CoreBuffeManager.hpp"
#include "Buffer/CoreBuffer.hpp"
#include "Device/VulkanDevice.hpp"
#include "Pipeline/Pipeline.hpp"
#include "../../Model/Model.hpp"
#include "../../entities/camera.hpp"
#include <glm\gtx\string_cast.hpp>

class SkySphere {
    GameCore::VertexLayout vertexLayout {{
        GameCore::VertexLayout::Component::VERTEX_COMPONENT_POSITION,
        GameCore::VertexLayout::Component::VERTEX_COMPONENT_NORMAL,
        GameCore::VertexLayout::Component::VERTEX_COMPONENT_UV,
    }};

    // Skysphere vertex shader stage
    struct {
        glm::mat4 mvp;
    } uboVS;

    const GameCore::VulkanDevice& context;
    GameCore::CoreBuffer          skySphereVertex;

    GameCore::Image skySphereTexture;

    vk::DescriptorSet skySphereDescriptorSet;

    vk::PipelineLayout      skyspherePipelineLayout;
    vk::Pipeline            skyspherePipeline;
    vk::DescriptorSetLayout skysphereDescriptorSetLayout;

    GameCore::CoreBufferManager bufferManager {context};
    const Camera&               _camera;

    // const SkySpherePipeline& skySpherePipeline;

    // TinyObjModelLoader				skySphereModel{ context };

    Model skySphereModel {context};

    GameCore::DescriptorSetLayoutBinding descSetLayout { };

   public:
    SkySphere (const GameCore::VulkanDevice& Context, const Camera& camera);

    void destroy( );
    void loadAssets( );
    void prepareUniformBuffers (const glm::mat4& perspective);
    void updateUniformBuffers (const glm::mat4& perspective);
    void updateDrawCommandBuffer (const vk::CommandBuffer& cmdBuffer);
    void descriptorDestroy( );
    void setupDescriptorSets (GameCore::DescriptorPool& descPool);
    void createPipelines (vk::RenderPass const& renderPass);
    void createDescriptorSetLayouts( );
};

#endif  // SKYSPHERE_HPP
