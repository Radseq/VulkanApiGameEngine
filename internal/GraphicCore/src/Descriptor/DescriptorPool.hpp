#ifndef DESCRIPTOR_POOL_HPP
#define DESCRIPTOR_POOL_HPP

#include <vector>
#include <vulkan/vulkan.hpp>

namespace GraphicCore {
    class DescriptorPool {
        uint32_t                            count {0};
        std::vector<vk::DescriptorPoolSize> descPoolSize;
        vk::DescriptorPool                  descriptorPool;

#ifdef DEBUG
        std::vector<std::pair<vk::DescriptorType, uint32_t>> typesCanBeUsed;

        void addTypesToPair (const std::pair<vk::DescriptorType, uint32_t>& pair);
#endif

       public:
        /// <summary>
        /// This array describes how many descriptors of a certain type will be allocated NOT in a single descriptor set
        /// but in total from a given pool
        /// </summary>
        /// <param name="descType"></param>
        /// <param name="descriptorCount">number of descriptors of a given type which can be allocated in total from a
        /// given pool (across all sets)</param>
        void addDescriptorPoolSize (const vk::DescriptorType& descType, const uint32_t& descriptorCount);

        /// <summary>
        /// create pool
        /// </summary>
        /// <param name="device">local device</param>
        /// <param name="descMaxSets">Specifies how many descriptor sets can be allocated from a given pool</param>
        void create (const vk::Device& device, const uint32_t& descMaxSets);

        const vk::DescriptorPool& getDescriptorPool( );
        void                      destroy (const vk::Device& device);

#ifdef DEBUG
        std::vector<std::pair<vk::DescriptorType, uint32_t>>& usedDescTypes( );
#endif
    };
}  // namespace GraphicCore
#endif  // DESCRIPTOR_POOL_HPP
