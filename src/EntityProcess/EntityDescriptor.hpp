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
    const GameCore::VulkanDevice&        context;
    GameCore::DescriptorPool             descriptorPool { };
    GameCore::DescriptorSetLayoutBinding descSetLayout { };
    GameCore::DescriptorSets             descriptorSets {descriptorPool, context.getVkDevice( )};

    uint32_t binding {0};
    uint32_t descSetIndex {0};

   public:
    explicit EntityDescriptor (const GameCore::VulkanDevice& Context);

    void createDescriptorSets (const uint32_t& descSetsCount);
    void createDescriptorPool (const uint32_t& poolSizeDescSetsCount);
    void createDescriptorSetLayout( );

    void destroy( );

    const GameCore::DescriptorSetLayoutBinding& getDescSetLayout( ) const;
    const GameCore::DescriptorSets&             getDescriptorSets( ) const;
    /*
    template <typename T, typename U> void createWriteDescriptors2 (std::vector<T>& t, std::vector<T>& t2, U& u)
    {
        for (size_t i = 0; i < 3; ++i)  // later to change
        {
            vk::WriteDescriptorSet descriptorWrite = { };
            descriptorWrite.dstSet                 = descriptorSets.getDescriptorSetByIndex (i);
            descriptorWrite.dstBinding             = 0;
            descriptorWrite.dstArrayElement        = 0;
            descriptorWrite.descriptorType         = vk::DescriptorType::eUniformBuffer;
            descriptorWrite.descriptorCount        = 1;
            descriptorWrite.pBufferInfo            = &t [i].getDescriptorInfo( );
            descriptorWrites2.push_back (descriptorWrite);
            // descriptorSets.addDescriptorWrite2 (descriptorWrite);

            vk::WriteDescriptorSet descriptorWrite2 = { };
            descriptorWrite2.dstSet                 = descriptorSets.getDescriptorSetByIndex (i);
            descriptorWrite2.dstBinding             = 1;
            descriptorWrite2.dstArrayElement        = 0;
            descriptorWrite2.descriptorType         = vk::DescriptorType::eCombinedImageSampler;
            descriptorWrite2.descriptorCount        = 1;
            descriptorWrite2.pImageInfo             = &u.getDescImageInfo( );
            descriptorWrites2.push_back (descriptorWrite2);
            // descriptorSets.addDescriptorWrite2 (descriptorWrite2);

            vk::WriteDescriptorSet descriptorWrite3 = { };
            descriptorWrite3.dstSet                 = descriptorSets.getDescriptorSetByIndex (i);
            descriptorWrite3.dstBinding             = 2;
            descriptorWrite3.dstArrayElement        = 0;
            descriptorWrite3.descriptorType         = vk::DescriptorType::eUniformBuffer;
            descriptorWrite3.descriptorCount        = 1;
            descriptorWrite3.pBufferInfo            = &t2 [i].getDescriptorInfo( );
            descriptorWrites2.push_back (descriptorWrite3);
            // descriptorSets.addDescriptorWrite2 (descriptorWrite3);

            // descriptorSets.update( );
            // descriptorSets.clearWriteDescSets( );
        }
    }*/

    template <typename T> void addDescriptorWrite (const T& src, const vk::DescriptorType& type)
    {
        vk::WriteDescriptorSet writeDescSet { };
        writeDescSet.dstSet          = descriptorSets.getDescriptorSetByIndex (descSetIndex);
        writeDescSet.dstBinding      = binding;
        writeDescSet.dstArrayElement = 0;
        writeDescSet.descriptorType  = type;

        if constexpr (std::is_same<T, vk::DescriptorBufferInfo>::value)
        {
            GameCore::util::updatePointerFromData (src, writeDescSet.descriptorCount, writeDescSet.pBufferInfo); 
        }
        else if constexpr (std::is_same<T, vk::DescriptorImageInfo>::value)
        {
            GameCore::util::updatePointerFromData (src, writeDescSet.descriptorCount, writeDescSet.pImageInfo);
        }
        else if constexpr (std::is_same<T, vk::BufferView>::value)
        {
            GameCore::util::updatePointerFromData (src, writeDescSet.descriptorCount, writeDescSet.pTexelBufferView);
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
