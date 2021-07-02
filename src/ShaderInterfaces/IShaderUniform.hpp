#ifndef ISHADER_UNIFORM_HPP
#define ISHADER_UNIFORM_HPP

#include <glm/glm.hpp>
#include <string_view>
#include <vector>

#include "../src/Device/VulkanDevice.hpp"

class IShaderUniform
{
   public:
    virtual ~IShaderUniform( ) = default;

    virtual void Create (const GraphicCore::VulkanDevice& context, const uint32_t& count) = 0;
    virtual void Destroy (const GraphicCore::VulkanDevice& context)                       = 0;
    virtual const vk::DescriptorBufferInfo& GetUniformDesc (const uint32_t& index)        = 0;
};

#endif  // ISHADER_UNIFORM_CREATE_HPP
