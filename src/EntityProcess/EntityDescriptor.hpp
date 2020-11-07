#ifndef ENTITY_DESCRIPTOR_HPP
#define ENTITY_DESCRIPTOR_HPP

#include <Descriptor/DescriptorSets.hpp>
#include <Descriptor\DescriptorPool.hpp>
#include <Device\VulkanDevice.hpp>
#include <Model\VertexLayout.hpp>
#include <Pipeline\PipelineLayout.hpp>

#include "Util/util.hpp"

class EntityDescriptor
{
    const GraphicCore::VulkanDevice&        context;
    GraphicCore::DescriptorPool             descriptorPool { };
    GraphicCore::DescriptorSetLayoutBinding descSetLayout { };
    GraphicCore::DescriptorSets             descriptorSets {descriptorPool, context.getVkDevice( )};

    uint32_t binding {0};
    uint32_t descSetIndex {0};

   public:
    explicit EntityDescriptor (const GraphicCore::VulkanDevice& Context);

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
        {
            GraphicCore::util::updatePointerFromData (src, writeDescSet.descriptorCount, writeDescSet.pBufferInfo); 
        }
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
};

#endif  // ENTITY_DESCRIPTOR_HPP
