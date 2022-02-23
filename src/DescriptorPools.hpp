#ifndef DESCRIPTOR_POOLS_HPP
#define DESCRIPTOR_POOLS_HPP

#include <DescriptorPool.hpp>
#include <DescriptorSetLayout.hpp>
#include <DescriptorSets.hpp>
#include <VulkanDevice.hpp>

#include "pch.h"

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
    void Destroy( );
};

#endif  // DESCRIPTOR_POOLS_HPP
