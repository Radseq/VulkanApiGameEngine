#ifndef ENTITY_SHADER_HPP
#define ENTITY_SHADER_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>
#include <glm\gtx\string_cast.hpp>

#include "../UniformBuffers/UniformBuffer.hpp"
#include "../Util/math.hpp"
#include "EntityDescriptor.hpp"
#include "EntityPipeline.hpp"

class EntityShader
{
    const GraphicCore::VulkanDevice& device;
    const GraphicCore::SwapChain&    swapChain;

    EntityPipeline          pipeline {device, swapChain};
    const EntityDescriptor& descriptor;

   public:
    EntityShader (const GraphicCore::VulkanDevice& Device, const GraphicCore::SwapChain& SwapChain,
                  const EntityDescriptor& EntityDescriptor);

    void init (const vk::RenderPass& renderPass);
    void bind (const vk::CommandBuffer& cmdBufer, const size_t& frameIndex);
};
#endif  // ENTITY_SHADER_HPP
