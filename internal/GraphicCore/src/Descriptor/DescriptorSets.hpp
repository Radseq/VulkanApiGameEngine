#ifndef DESCRIPTOR_SETS_HPP
#define DESCRIPTOR_SETS_HPP

#include <Descriptor\DescriptorPool.hpp>
#include <vulkan/vulkan.hpp>

#include "DescriptorSetLayout.hpp"

namespace GraphicCore
{
    class DescriptorSets
    {
        GraphicCore::DescriptorPool&        descPool;
        const vk::Device&                   device;
        std::vector<vk::DescriptorSet>      descriptorSets   = { };
        std::vector<vk::WriteDescriptorSet> descriptorWrites = { };

        // uint32_t binding {0};

        // void assingVkDescSet( );

#ifdef DEBUG
        void validateWriteDescSet (const vk::WriteDescriptorSet& writeDescSet);
#endif
       public:
        DescriptorSets (GraphicCore::DescriptorPool& DescPool, const vk::Device& Device)
            : descPool (DescPool)
            , device (Device) { };

        void create (const uint32_t& count, const DescriptorSetLayoutBinding& descSetLayout);

        void update( );

        const vk::DescriptorSet& getDescriptorSetByIndex (const size_t& i) const;

        //void clearWriteDescSets( );
        void addDescriptorWrite (const vk::WriteDescriptorSet&& writeDescSet);

        const uint32_t GetDescSetCount( ) const;
    };
}  // namespace GraphicCore
#endif  // DESCRIPTOR_SETS_HPP
