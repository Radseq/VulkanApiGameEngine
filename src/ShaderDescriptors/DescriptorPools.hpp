#ifndef DESCRIPTOR_POOLS_HPP
#define DESCRIPTOR_POOLS_HPP

#include <Descriptor/DescriptorPool.hpp>
#include <Descriptor/DescriptorSetLayout.hpp>
#include <Descriptor/DescriptorSets.hpp>
#include <Device/VulkanDevice.hpp>
#include <array>
#include <string_view>
#include <typeinfo>

#include "../Base/Core.hpp"

class DescriptorPools
{
    GraphicCore::DescriptorSetLayoutBinding& m_DescSetLayout;
    const vk::Device&                        m_Device;

    std::vector<GraphicCore::DescriptorPool> m_DescPools;

   public:
    DescriptorPools (GraphicCore::DescriptorSetLayoutBinding& DescSetLayout, const vk::Device& Device)
        : m_DescSetLayout (DescSetLayout)
        , m_Device (Device) { };

    void SetDescSetLayout (const GraphicCore::DescriptorSetLayoutBinding& DescSetLayout);
    void CreateDescriptorPool (const uint32_t& descCountGivenPool, const uint32_t& maxSets);
    void CreateDescriptorPool (const std::vector<uint32_t>& descCountGivenPool, const uint32_t& maxSets);

    VulkanGame::Ref<GraphicCore::DescriptorSets> CreateDescSet (const uint32_t& descSetsCount);
};

#endif  // DESCRIPTOR_POOLS_HPP
