#ifndef ENTITY_RENDERER_HPP
#define ENTITY_RENDERER_HPP

#include <Buffer\CoreBuffeManager.hpp>
#include <Buffer\CoreBuffer.hpp>
#include <Descriptor\DescriptorPool.hpp>
#include <Descriptor\DescriptorSetLayout.hpp>
#include <Descriptor\DescriptorSets.hpp>
#include <Framebuffer\SwapChain.hpp>
#include <Image\CoreImageAdv.hpp>
#include <Image\CoreImageView.hpp>
#include <Image\Image.hpp>
#include <Image\ImageManager.hpp>
#include <Model\VertexLayout.hpp>
#include <Pipeline/Pipeline.hpp>
#include <Pipeline\PipelineLayout.hpp>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include "../FileLoader/AssimpModelLoader.hpp"
#include "../FileLoader/TinyObjModelLoader.hpp"
#include "../FileLoader/stbImgFileLoader.hpp"
#include "../Model/TexturedModel.hpp"
#include "../Textures/Texture2D.hpp"
#include "../Util/math.hpp"
#include "../entities/camera.hpp"
#include "../entities/entity.hpp"
#include "../entities/light.hpp"
#include "EntityDescriptor.hpp"
#include "EntityPipeline.hpp"
#include "EntityShader.hpp"
#include "glm/ext.hpp"

class EntityRenderer
{
    const Light&                  light;
    const GameCore::VulkanDevice& context;
    const GameCore::SwapChain&    swapChain;

    struct UniformBufferObject
    {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
        glm::vec3 lightPosition;
    };

    struct lightUniformBufferObject
    {
        glm::vec3 lightColor;
        float     shineDamper;
        float     reflectivity;
    };

    Camera&       camera;
    UniformBuffer uniform;
    UniformBuffer uniform2;

    using EntityMap = std::unordered_map<TexturedModel const*, std::vector<Entity const*>>;

    const EntityMap* entities;

    EntityDescriptor entityDesc {context};
    EntityShader     shader {context, swapChain, entityDesc};

   public:
    EntityRenderer (const GameCore::VulkanDevice& Context, const GameCore::SwapChain& SwapChain, Camera& _camera,
                    const Light& Light);

    void create (const vk::RenderPass& renderPass);

    void destroy (bool isSwapChainCleanUp = false);

    void createUniformBuffers( );
    void updateUniformBuffer (const uint32_t& currentImage, const glm::mat4& proj);
    void updateDrawCommandBuffer (const vk::CommandBuffer& cmdBufer, const size_t& i);

    void pushEntity (const EntityMap* Entities);
};

#endif  // ENTITY_RENDERER_HPP