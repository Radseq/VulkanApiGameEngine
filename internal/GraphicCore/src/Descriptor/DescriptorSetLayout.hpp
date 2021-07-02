#ifndef DESCRIPTOR_SET_LAYOUT_HPP
#define DESCRIPTOR_SET_LAYOUT_HPP

#include <Util\math.hpp>
#include <unordered_map>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace GraphicCore
{
    class DescriptorSetLayoutBinding
    {
        uint8_t                                     m_BindingCount {0};
        std::vector<vk::DescriptorSetLayoutBinding> m_Bindings;
        vk::DescriptorSetLayout                     m_DescriptorSetLayout;

        std::unordered_map<vk::DescriptorType, std::vector<vk::ShaderStageFlagBits>> m_TypesShader;

        std::vector<vk::DescriptorType> m_DescTypes;

        const vk::Flags<vk::ShaderStageFlagBits> ParseVecFlags (
            const std::vector<vk::ShaderStageFlagBits>& shaderStageFlag);

       public:
        void addDescriptorSetLayoutBinding (const vk::DescriptorType&&                   descType,
                                            const std::vector<vk::ShaderStageFlagBits>&& shaderStageFlag);

        void create (const vk::Device& device);
        void destroy (const vk::Device& device);

        const vk::DescriptorSetLayout& getDescriptorSetLayout( ) const;

        const std::unordered_map<vk::DescriptorType, std::vector<vk::ShaderStageFlagBits>> GetDescTypeWithFlags( )
            const;

        const std::vector<vk::DescriptorType>& GetDescTypes( );
    };
}  // namespace GraphicCore
#endif  // DESCRIPTOR_SET_LAYOUT_HPP
