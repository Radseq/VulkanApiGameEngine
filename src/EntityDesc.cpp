#include "pch.h"
#include "EntityDesc.hpp"

EntityDesc::EntityDesc (const GraphicCore::VulkanDevice& Context, const vk::DescriptorImageInfo& descImgInfo)
    : m_Context (Context)
    , m_DescImgInfo (descImgInfo)
{
    ShadersVec.push_back (new LightShader( ));
    ShadersVec.push_back (new EntSpecLightShader( ));
}

const GraphicCore::DescriptorSetLayoutBinding&     EntityDesc::GetDescSetLayout( ) const { return m_DescSetLayout; }
const VulkanGame::Ref<GraphicCore::DescriptorSets> EntityDesc::GetDescSets( ) const { return m_DescSet; }

std::vector<IShader*> EntityDesc::GetShaders( ) { return ShadersVec; };

void EntityDesc::UpdateWriteDescriptors( )
{
    m_DescSet->update( );
    // m_DescSet->clearWriteDescSets( );

    binding = 0;

    ResetDescSetIndex( );
}

void EntityDesc::CreateDescriptorSets (const uint32_t& count)
{
    descPools.CreateDescriptorPool (count, count);
    m_DescSet  = descPools.CreateDescSet (count);
    maxDescSet = m_DescSet->GetDescSetCount( ) - 1;

    for (const auto& shader : ShadersVec) { shader->Create (m_Context, count); }

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
        WriteDescBufferInfo (&ShadersVec[1]->GetUniformDesc (i));

        WriteDescImgInfo (&m_DescImgInfo);

        WriteDescBufferInfo (&ShadersVec[0]->GetUniformDesc (i));
        UpdateWriteDescriptors( );
    }
}

void EntityDesc::Destroy( )
{
    for (const auto shader : ShadersVec)
    {
        shader->Destroy (m_Context);
        delete shader;
    }
    
    ShadersVec.clear( );

    descPools.Destroy( );
}

void EntityDesc::WriteDescBufViewInfo (const vk::BufferView* BuffeeView)
{
    vk::WriteDescriptorSet writeDescSet { };
    writeDescSet.dstSet          = m_DescSet->getDescriptorSetByIndex (descSetIndex);
    writeDescSet.dstBinding      = binding;
    writeDescSet.dstArrayElement = 0;
    writeDescSet.descriptorType  = m_DescSetLayout.GetDescTypes( ) [2];
    GraphicCore::Util::updatePointerFromData (*BuffeeView, writeDescSet.descriptorCount, writeDescSet.pTexelBufferView);
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
    GraphicCore::Util::updatePointerFromData (*DescImgInfo, writeDescSet.descriptorCount, writeDescSet.pImageInfo);
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
    GraphicCore::Util::updatePointerFromData (*DescBuffInfo, writeDescSet.descriptorCount, writeDescSet.pBufferInfo);
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