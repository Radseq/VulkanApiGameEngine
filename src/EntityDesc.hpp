#ifndef ENTITY_DESC_HPP
#define ENTITY_DESC_HPP

#include <DescriptorSets.hpp>

#include "DescriptorPools.hpp"
#include "EntSpecLightShader.hpp"
#include "IShader.hpp"
#include "IShaderDescSet.hpp"
#include "IShaderPipeline.hpp"
#include "LightShader.hpp"
#include "entity.hpp"
#include "pch.h"

class EntityDesc : public IShaderDescSet
{
    GraphicCore::DescriptorSetLayoutBinding m_DescSetLayout { };
    const vk::DescriptorImageInfo&          m_DescImgInfo;
    const GraphicCore::VulkanDevice&        m_Context;

    DescriptorPools                              descPools {m_DescSetLayout, m_Context.getVkDevice( )};
    VulkanGame::Ref<GraphicCore::DescriptorSets> m_DescSet;

    uint32_t binding {0};  // how much addDescriptorWrite called
    uint32_t descSetIndex {0};
    uint32_t maxDescSet {0};

    std::vector<IShader*> ShadersVec;

    void ResetDescSetIndex( );

    void WriteDescBufferInfo (const vk::DescriptorBufferInfo* DescBuffInfo);
    void WriteDescImgInfo (const vk::DescriptorImageInfo* DescImgInfo);
    void WriteDescBufViewInfo (const vk::BufferView* BuffeeView);

   public:
    EntityDesc (const GraphicCore::VulkanDevice& Context, const vk::DescriptorImageInfo& descImgInfo);

    void CreateDescriptorSets (const uint32_t& count) override;

    // Can be in separate interface,
    // but desc set must be for specyfied layout and there will be
    // not same layout for diffrent set, at last for now i dont see.
    void CreateDescriptorSetLayout( ) override;

    void WriteDescSet( ) override;

    void UpdateWriteDescriptors( ) override;

    const GraphicCore::DescriptorSetLayoutBinding& GetDescSetLayout( ) const override;

    const VulkanGame::Ref<GraphicCore::DescriptorSets> GetDescSets( ) const override;

    std::vector<IShader*> GetShaders( ) override;

    void Destroy( ) override;
};
#endif  // ENTITY_DESC_HPP
