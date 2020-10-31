#ifndef DESCRIPTOR_SETS_HPP
#define DESCRIPTOR_SETS_HPP

#include <Descriptor\DescriptorPool.hpp>
#include <vulkan/vulkan.hpp>

#include "DescriptorSetLayout.hpp"

namespace GameCore
{
    class DescriptorSets
    {
        GameCore::DescriptorPool&           descPool;
        const vk::Device&                   device;
        std::vector<vk::DescriptorSet>      descriptorSets   = { };
        std::vector<vk::WriteDescriptorSet> descriptorWrites = { };

        // uint32_t binding {0};

        // void assingVkDescSet( );

#ifdef DEBUG
        void validateWriteDescSet (const vk::WriteDescriptorSet& writeDescSet);
#endif
       public:
        DescriptorSets (GameCore::DescriptorPool& DescPool, const vk::Device& Device)
            : descPool (DescPool)
            , device (Device) { };

        void create (const vk::Device& device, const uint32_t& count, DescriptorSetLayoutBinding& descSetLayout);

        // dont work, because in update device updateDescriptorSets wont update somehow (in renderer is error from vulkan
        // that desc is not updated)
        /*
        template <typename T> void addDescriptorWrite (const T& src, const vk::DescriptorType& type) {
            vk::WriteDescriptorSet writeDescSet { };
            writeDescSet.dstBinding      = binding;
            writeDescSet.descriptorType  = type;
            writeDescSet.dstArrayElement = 0;
            if constexpr (std::is_same<T, vk::DescriptorBufferInfo>::value) {
                util::updatePointerFromData (src, writeDescSet.descriptorCount, writeDescSet.pBufferInfo);
            } else if constexpr (std::is_same<T, vk::DescriptorImageInfo>::value) {
                util::updatePointerFromData (src, writeDescSet.descriptorCount, writeDescSet.pImageInfo);
            } else if constexpr (std::is_same<T, vk::BufferView>::value) {
                util::updatePointerFromData (src, writeDescSet.descriptorCount, writeDescSet.pTexelBufferView);
            }
            descriptorWrites.push_back (writeDescSet);
#ifdef DEBUG
            validateWriteDescSet (writeDescSet);
#endif
            ++binding;
        }*/

        void update( );
        // void assingVkDescSet( );

        const vk::DescriptorSet& getDescriptorSetByIndex (const size_t& i) const;

        // const std::vector<vk::WriteDescriptorSet>& getWriteDescSets( );

        void clearWriteDescSets( );
        void addDescriptorWrite (const vk::WriteDescriptorSet& writeDescSet);
    };
}  // namespace GameCore
#endif  // DESCRIPTOR_SETS_HPP
