#ifndef ENTITY_SHADER2_HPP
#define ENTITY_SHADER2_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>
#include <glm\gtx\string_cast.hpp>

#include "../UniformBuffers/UniformBuffer.hpp"
#include "../Util/math.hpp"
#include "EntityDescriptor.hpp"
#include "EntityPipeline.hpp"

class EntityShader2
{
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

    UniformBufferObject      ubo  = { };
    lightUniformBufferObject lubo = { };

    UniformBuffer uniform;
    UniformBuffer uniform2;

    const GraphicCore::VulkanDevice& m_Device;
    const vk::RenderPass&         m_RenderPass;
    const vk::Extent2D&           m_SwapChainWindowSize;

    // pipeline
    // must be in correct order of model vertexCoreBuffer loaded(from file)
    GraphicCore::VertexLayout vertex_layout {{GraphicCore::VertexLayout::Component::VERTEX_COMPONENT_POSITION,
                                           GraphicCore::VertexLayout::Component::VERTEX_COMPONENT_TEXT_COORD,
                                           GraphicCore::VertexLayout::Component::VERTEX_COMPONENT_NORMAL}};

    vk::Pipeline             graphicsPipeline;
    GraphicCore::PipelineLayout pipelineLayout { };
    // end pipeline

    // descriptor

    GraphicCore::DescriptorPool             descriptorPool { };
    GraphicCore::DescriptorSetLayoutBinding descSetLayout { };
    GraphicCore::DescriptorSets             descriptorSets {descriptorPool, m_Device.getVkDevice( )};

    uint32_t binding {0};
    uint32_t descSetIndex {0};

    // end descriptor

   public:
    EntityShader2 (const GraphicCore::VulkanDevice& Device, const vk::RenderPass& renderPass,
                   const vk::Extent2D& swapChainWindowSize);

    void init (const uint32_t& frames);
    void bind (const vk::CommandBuffer& cmdBufer, const size_t& frameIndex);

    // pipeline

    void createGraphicsPipeline (const GraphicCore::DescriptorSetLayoutBinding& descSetLayout);

    // end pipeline

    // descriptor

    void createDescriptorSets (const uint32_t& descSetsCount);
    void createDescriptorPool (const uint32_t& poolSizeDescSetsCount);
    void createDescriptorSetLayout( );

    void destroy( );

    const GraphicCore::DescriptorSetLayoutBinding& getDescSetLayout( ) const;
    const GraphicCore::DescriptorSets&             getDescriptorSets( ) const;

    template <typename T> void addDescriptorWrite (const T& src, const vk::DescriptorType& type)
    {
        vk::WriteDescriptorSet writeDescSet { };
        writeDescSet.dstSet          = descriptorSets.getDescriptorSetByIndex (descSetIndex);
        writeDescSet.dstBinding      = binding;
        writeDescSet.dstArrayElement = 0;
        writeDescSet.descriptorType  = type;

        if constexpr (std::is_same<T, vk::DescriptorBufferInfo>::value)
        { GraphicCore::util::updatePointerFromData (src, writeDescSet.descriptorCount, writeDescSet.pBufferInfo); }
        else if constexpr (std::is_same<T, vk::DescriptorImageInfo>::value)
        {
            GraphicCore::util::updatePointerFromData (src, writeDescSet.descriptorCount, writeDescSet.pImageInfo);
        }
        else if constexpr (std::is_same<T, vk::BufferView>::value)
        {
            GraphicCore::util::updatePointerFromData (src, writeDescSet.descriptorCount, writeDescSet.pTexelBufferView);
        }

        descriptorSets.addDescriptorWrite (writeDescSet);

        // todo explain
        if (descSetIndex == 2 && binding == 2) descSetIndex = 0;

        if (binding == 2)
        {
            binding = 0;
            ++descSetIndex;
        }
        else
            ++binding;
    }

    void updateWriteDescriptors( );

    // end descriptor
};
#endif  // ENTITY_SHADER2_HPP
