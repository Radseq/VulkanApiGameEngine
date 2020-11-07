#ifndef DESCRIPTOR_SET_LAYOUT_HPP
#define DESCRIPTOR_SET_LAYOUT_HPP

#include <Util\math.hpp>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace GraphicCore {
    class DescriptorSetLayoutBinding {
        uint8_t                                     bindingCount {0};
        std::vector<vk::DescriptorSetLayoutBinding> bindings;
        vk::DescriptorSetLayout                     descriptorSetLayout;

#ifdef DEBUG
        void validateLayoutTypeWithDescPoolTypes (const std::vector<vk::DescriptorType>& typesCanBeUsed);
#endif

       public:
        void addDescriptorSetLayoutBinding (const vk::DescriptorType&      descType,
                                            const vk::ShaderStageFlagBits& shaderStageFlag);
        void addDescriptorSetLayoutBinding (const vk::DescriptorType&                 descType,
                                            const vk::Flags<vk::ShaderStageFlagBits>& shaderStageFlag);

        void create (const vk::Device& device, const std::vector<vk::DescriptorType>& typesCanBeUsed);
        void destroy (const vk::Device& device);

        const vk::DescriptorSetLayout& getDescriptorSetLayout( ) const;
    };
}  // namespace GraphicCore
#endif  // DESCRIPTOR_SET_LAYOUT_HPP
