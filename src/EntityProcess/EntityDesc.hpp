#ifndef ENTITY_DESC_HPP
#define ENTITY_DESC_HPP

#include <Descriptor/DescriptorSets.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>
#include <glm\gtx\string_cast.hpp>

#include "../Shader/EntSpecLightShader.hpp"
#include "../Shader/LightShader.hpp"
#include "../ShaderDescInterfaces/IShaderDescSet.hpp"
#include "../ShaderDescInterfaces/IShaderPipeline.hpp"
#include "../ShaderDescriptors/DescriptorPools.hpp"
#include "../ShaderInterfaces/IShader.hpp"
#include "../Util/math.hpp"
#include "../entities/entity.hpp"

class EntityDesc : public IShaderDescSet
{
    GraphicCore::DescriptorSetLayoutBinding m_DescSetLayout { };
    const vk::DescriptorImageInfo&          m_DescImgInfo;
    const GraphicCore::VulkanDevice&        m_Context;

    DescriptorPools                              descPools {m_DescSetLayout, m_Context.getVkDevice( )};
    VulkanGame::Ref<GraphicCore::DescriptorSets> m_DescSet;

    VulkanGame::Ref<LightShader>        m_LightShader;
    VulkanGame::Ref<EntSpecLightShader> m_EntSpecLight;

    uint32_t binding {0};  // how much addDescriptorWrite called
    uint32_t descSetIndex {0};
    uint32_t maxDescSet {0};

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

    std::vector<VulkanGame::Ref<IShader>> GetShaders( ) override;

    void Destroy( ) override;
};
#endif  // ENTITY_DESC_HPP
