#include "DescriptorPools.hpp"

void DescriptorPools::SetDescSetLayout (const GraphicCore::DescriptorSetLayoutBinding& DescSetLayout)
{
    m_DescSetLayout = DescSetLayout;
}

void DescriptorPools::CreateDescriptorPool (const uint32_t& descCountGivenPool, const uint32_t& maxSets)
{
    assert (m_DescSetLayout.getDescriptorSetLayout( ));
    auto unorderedMap = m_DescSetLayout.GetDescTypeWithFlags( );

    GraphicCore::DescriptorPool descriptorPool { };

    for (const auto& data : unorderedMap) { descriptorPool.addDescriptorPoolSize (data.first, descCountGivenPool); }

    descriptorPool.create (m_Device, maxSets);

    m_DescPools.push_back (descriptorPool);
}

void DescriptorPools::CreateDescriptorPool (const std::vector<uint32_t>& sizeOfDescTypes, const uint32_t& maxSets)
{
    assert (m_DescSetLayout.getDescriptorSetLayout( ));
    auto unorderedMap = m_DescSetLayout.GetDescTypeWithFlags( );

    uint32_t iterator {0};

    assert (sizeOfDescTypes.size( ) == unorderedMap.size( ));

    GraphicCore::DescriptorPool descriptorPool { };

    for (const auto& data : unorderedMap)
    {
        descriptorPool.addDescriptorPoolSize (data.first, sizeOfDescTypes [iterator]);
        ++iterator;
    }

    descriptorPool.create (m_Device, maxSets);

    m_DescPools.push_back (descriptorPool);
}

VulkanGame::Ref<GraphicCore::DescriptorSets> DescriptorPools::CreateDescSet (const uint32_t& descSetsCount)
{
    assert (m_DescSetLayout.getDescriptorSetLayout( ));

    // we got last descriptor pool, because desc set should be created after creating pool and "last" desc pool will use
    // specyfic desc types like this desc set, we can always create find func for pool that match pool desc type with
    // m_DescSetLayout desc types
    auto lastCreatedPool = m_DescPools [m_DescPools.size( ) - 1];

    auto result = VulkanGame::CreateRef<GraphicCore::DescriptorSets> (lastCreatedPool, m_Device);
    result->create (descSetsCount, m_DescSetLayout);
    return result;
}
