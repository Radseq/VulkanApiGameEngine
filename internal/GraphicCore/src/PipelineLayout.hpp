#ifndef PIPLINE_LAYOUT_HPP
#define PIPLINE_LAYOUT_HPP

#include <DescriptorSetLayout.hpp>
#include <VulkanDevice.hpp>

namespace GraphicCore
{
    class PipelineLayout
    {
        vk::PipelineLayout               pipelineLayout;
        vk::PipelineLayoutCreateInfo     pipelineLayoutInfo = { };
        const GraphicCore::VulkanDevice& m_Device;

       public:
        PipelineLayout (const GraphicCore::VulkanDevice& Device)
            : m_Device (Device) { };

        void setPushConstant (const vk::PushConstantRange& pushConstantRange);
        void setPushConstant (const std::vector<vk::PushConstantRange>& pushConstantRange);

        void create (const DescriptorSetLayoutBinding& descSetLayout);

        const vk::PipelineLayout& getVkPipelineLayout( ) const;
    };
}  // namespace GraphicCore

#endif  // PIPLINE_LAYOUT_HPP
