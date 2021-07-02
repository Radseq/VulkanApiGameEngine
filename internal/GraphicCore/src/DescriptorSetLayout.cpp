#include "pch.h"
#include "DescriptorSetLayout.hpp"
namespace GraphicCore
{
    const vk::Flags<vk::ShaderStageFlagBits> DescriptorSetLayoutBinding::ParseVecFlags (
        const std::vector<vk::ShaderStageFlagBits>& shaderStageFlag)
    {
        vk::Flags<vk::ShaderStageFlagBits> flagsToUse;

        for (const auto& flag : shaderStageFlag) { flagsToUse = flagsToUse | flag; }
        return flagsToUse;
    }

    void DescriptorSetLayoutBinding::addDescriptorSetLayoutBinding (
        const vk::DescriptorType&& descType, const std::vector<vk::ShaderStageFlagBits>&& shaderStageFlag)
    {
        m_TypesShader [descType] = shaderStageFlag;
        m_DescTypes.push_back (descType);

        vk::DescriptorSetLayoutBinding uboLayoutBinding = { };
        uboLayoutBinding.binding                        = m_BindingCount;
        uboLayoutBinding.descriptorCount                = 1;
        uboLayoutBinding.descriptorType                 = descType;
        uboLayoutBinding.pImmutableSamplers             = nullptr;
        uboLayoutBinding.stageFlags                     = ParseVecFlags (shaderStageFlag);

        m_Bindings.push_back (uboLayoutBinding);

        ++m_BindingCount;
    }

    void DescriptorSetLayoutBinding::create (const vk::Device& device)
    {
        vk::DescriptorSetLayoutCreateInfo layoutInfo = { };
        layoutInfo.bindingCount                      = m_BindingCount;
        layoutInfo.pBindings                         = m_Bindings.data( );

        m_DescriptorSetLayout = device.createDescriptorSetLayout (layoutInfo);
    }

    const vk::DescriptorSetLayout& DescriptorSetLayoutBinding::getDescriptorSetLayout( ) const
    {
        assert (m_DescriptorSetLayout);
        return m_DescriptorSetLayout;
    }

    /// <summary>
    /// In pipeline desc types are using, like flags
    /// used to make validation
    /// </summary>
    /// <returns></returns>
    const std::unordered_map<vk::DescriptorType, std::vector<vk::ShaderStageFlagBits>>
    DescriptorSetLayoutBinding::GetDescTypeWithFlags( ) const
    {
        return m_TypesShader;
    }

    const std::vector<vk::DescriptorType>& DescriptorSetLayoutBinding::GetDescTypes( ) { return m_DescTypes; }

    void DescriptorSetLayoutBinding::destroy (const vk::Device& device)
    {
        device.destroyDescriptorSetLayout (m_DescriptorSetLayout);
    }
}  // namespace GraphicCore