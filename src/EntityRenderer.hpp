#ifndef ENTITY_RENDERER_HPP
#define ENTITY_RENDERER_HPP

#include <variant>

#include "EntityMeshRender.hpp"
#include "EntityRenderer.hpp"
#include "SwapChain.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "pch.h"

class EntityRenderer
{
    const Light&                     light;
    const GraphicCore::VulkanDevice& context;
    const GraphicCore::SwapChain&    swapChain;

    const Camera& camera;

    VulkanGame::Ref<EntityMeshRender> entities;

   public:
    EntityRenderer (const GraphicCore::VulkanDevice& Context, const GraphicCore::SwapChain& SwapChain,
                    const Camera& _camera, const Light& Light);

    void destroy (bool isSwapChainCleanUp = false);

    void createUniformBuffers (const glm::mat4& perspective);
    void updateUniformBuffer (const uint32_t& currentImage, const glm::mat4& proj);
    void updateDrawCommandBuffer (const vk::CommandBuffer& cmdBufer, const size_t& i);

    void pushEntity (VulkanGame::Ref<EntityMeshRender> Entities);
};

#endif  // ENTITY_RENDERER_HPP
