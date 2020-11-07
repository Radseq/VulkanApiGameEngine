#include "DescriptorSetLayout.hpp"
namespace GraphicCore
{
    void DescriptorSetLayoutBinding::addDescriptorSetLayoutBinding (const vk::DescriptorType&      descType,
                                                                    const vk::ShaderStageFlagBits& shaderStageFlag)
    {
        const auto flags = vk::Flags<vk::ShaderStageFlagBits> (shaderStageFlag);
        addDescriptorSetLayoutBinding (descType, flags);
    }

    void DescriptorSetLayoutBinding::addDescriptorSetLayoutBinding (
        const vk::DescriptorType& descType, const vk::Flags<vk::ShaderStageFlagBits>& shaderStageFlag)
    {
        vk::DescriptorSetLayoutBinding uboLayoutBinding = { };
        uboLayoutBinding.binding                        = bindingCount;
        uboLayoutBinding.descriptorCount                = 1;
        uboLayoutBinding.descriptorType                 = descType;
        uboLayoutBinding.pImmutableSamplers             = nullptr;
        uboLayoutBinding.stageFlags                     = shaderStageFlag;

        bindings.push_back (uboLayoutBinding);

        ++bindingCount;
    }

    void DescriptorSetLayoutBinding::create (const vk::Device&                      device,
                                             const std::vector<vk::DescriptorType>& typesCanBeUsed)
    {
#ifdef DEBUG
        validateLayoutTypeWithDescPoolTypes (typesCanBeUsed);
#endif
        vk::DescriptorSetLayoutCreateInfo layoutInfo = { };
        layoutInfo.bindingCount                      = GraphicCore::util::to_uint_32_t (bindings.size( ));
        layoutInfo.pBindings                         = bindings.data( );

        descriptorSetLayout = device.createDescriptorSetLayout (layoutInfo);
    }

    const vk::DescriptorSetLayout& DescriptorSetLayoutBinding::getDescriptorSetLayout( ) const
    {
        assert (descriptorSetLayout);
        return descriptorSetLayout;
    }

    void DescriptorSetLayoutBinding::destroy (const vk::Device& device)
    {
        device.destroyDescriptorSetLayout (descriptorSetLayout);
    }

#ifdef DEBUG
    void DescriptorSetLayoutBinding::validateLayoutTypeWithDescPoolTypes (
        const std::vector<vk::DescriptorType>& typesCanBeUsed)
    {
        uint32_t usedDescriptorType {0};

        for (const auto& descBinding : bindings)
        {
            for (const auto& type : typesCanBeUsed)
            {
                if (descBinding.descriptorType == type)
                {
                    ++usedDescriptorType;
                    break;
                }
            }
        }

        assert (usedDescriptorType == bindingCount);
    }
#endif
}  // namespace GraphicCore