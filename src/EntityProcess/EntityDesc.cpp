#include "EntityDesc.hpp"

EntityDesc::EntityDesc (const GraphicCore::VulkanDevice& Context, const vk::DescriptorImageInfo& descImgInfo)
    : m_Context (Context)
    , m_DescImgInfo (descImgInfo)
{
    m_LightShader  = VulkanGame::CreateRef<LightShader>( );
    m_EntSpecLight = VulkanGame::CreateRef<EntSpecLightShader>( );
}

const GraphicCore::DescriptorSetLayoutBinding&     EntityDesc::GetDescSetLayout( ) const { return m_DescSetLayout; }
const VulkanGame::Ref<GraphicCore::DescriptorSets> EntityDesc::GetDescSets( ) const { return m_DescSet; }

std::vector<VulkanGame::Ref<IShader>> EntityDesc::GetShaders( )
{
    std::vector<VulkanGame::Ref<IShader>> shader;
    shader.push_back (m_LightShader);
    shader.push_back (m_EntSpecLight);
    return shader;
};

void EntityDesc::UpdateWriteDescriptors( )
{
    m_DescSet->update( );
    //m_DescSet->clearWriteDescSets( );

    binding = 0;

    ResetDescSetIndex( );
}

void EntityDesc::CreateDescriptorSets (const uint32_t& count)
{
    descPools.CreateDescriptorPool (count, count);
    m_DescSet  = descPools.CreateDescSet (count);
    maxDescSet = m_DescSet->GetDescSetCount( ) - 1;

    m_EntSpecLight->Create (m_Context, count);
    m_LightShader->Create (m_Context, count);

    WriteDescSet( );
}

void EntityDesc::CreateDescriptorSetLayout( )
{
    m_DescSetLayout.addDescriptorSetLayoutBinding (vk::DescriptorType::eUniformBuffer,
                                                   {vk::ShaderStageFlagBits::eVertex});
    m_DescSetLayout.addDescriptorSetLayoutBinding (vk::DescriptorType::eCombinedImageSampler,
                                                   {vk::ShaderStageFlagBits::eFragment});
    m_DescSetLayout.addDescriptorSetLayoutBinding (vk::DescriptorType::eUniformBuffer,
                                                   {vk::ShaderStageFlagBits::eFragment});

    m_DescSetLayout.create (m_Context.getVkDevice( ));
}

void EntityDesc::WriteDescSet( )
{
    for (uint32_t i = 0; i < m_DescSet->GetDescSetCount( ); ++i)
    {
        WriteDescBufferInfo (&m_EntSpecLight->GetUniformDesc (i));

        WriteDescImgInfo (&m_DescImgInfo);

        WriteDescBufferInfo (&m_LightShader->GetUniformDesc (i));
        UpdateWriteDescriptors( );
    }
}

void EntityDesc::Destroy( )
{
    m_EntSpecLight->Destroy (m_Context);
    m_LightShader->Destroy (m_Context);
}

void EntityDesc::WriteDescBufViewInfo (const vk::BufferView* BuffeeView)
{
    vk::WriteDescriptorSet writeDescSet { };
    writeDescSet.dstSet          = m_DescSet->getDescriptorSetByIndex (descSetIndex);
    writeDescSet.dstBinding      = binding;
    writeDescSet.dstArrayElement = 0;
    writeDescSet.descriptorType  = m_DescSetLayout.GetDescTypes( ) [2];
    GraphicCore::util::updatePointerFromData (*BuffeeView, writeDescSet.descriptorCount, writeDescSet.pTexelBufferView);
    m_DescSet->addDescriptorWrite (std::move (writeDescSet));
    ++binding;
}

void EntityDesc::WriteDescImgInfo (const vk::DescriptorImageInfo* DescImgInfo)
{
    vk::WriteDescriptorSet writeDescSet { };
    writeDescSet.dstSet          = m_DescSet->getDescriptorSetByIndex (descSetIndex);
    writeDescSet.dstBinding      = binding;
    writeDescSet.dstArrayElement = 0;
    writeDescSet.descriptorType  = m_DescSetLayout.GetDescTypes( ) [1];
    GraphicCore::util::updatePointerFromData (*DescImgInfo, writeDescSet.descriptorCount, writeDescSet.pImageInfo);
    m_DescSet->addDescriptorWrite (std::move (writeDescSet));
    ++binding;
}

void EntityDesc::WriteDescBufferInfo (const vk::DescriptorBufferInfo* DescBuffInfo)
{
    vk::WriteDescriptorSet writeDescSet { };
    writeDescSet.dstSet          = m_DescSet->getDescriptorSetByIndex (descSetIndex);
    writeDescSet.dstBinding      = binding;
    writeDescSet.dstArrayElement = 0;
    writeDescSet.descriptorType  = m_DescSetLayout.GetDescTypes( ) [0];
    GraphicCore::util::updatePointerFromData (*DescBuffInfo, writeDescSet.descriptorCount, writeDescSet.pBufferInfo);
    m_DescSet->addDescriptorWrite (std::move (writeDescSet));
    ++binding;
}

void EntityDesc::ResetDescSetIndex( )
{
    if (descSetIndex == maxDescSet) { descSetIndex = 0; }
    else
    {
        ++descSetIndex;
    }
}