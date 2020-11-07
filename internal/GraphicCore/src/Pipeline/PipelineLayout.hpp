#ifndef PIPLINE_LAYOUT_HPP
#define PIPLINE_LAYOUT_HPP

#include <Descriptor\DescriptorSetLayout.hpp>
#include <Util/util.hpp>
#include <vulkan/vulkan.hpp>

namespace GraphicCore
{
    class PipelineLayout
    {
        vk::PipelineLayout           pipelineLayout;
        vk::PipelineLayoutCreateInfo pipelineLayoutInfo = { };

       public:
        void setPushConstant (const vk::PushConstantRange& pushConstantRange);
        void setPushConstant (const std::vector<vk::PushConstantRange>& pushConstantRange);

        void create (const vk::Device& device, const DescriptorSetLayoutBinding& descSetLayout);

        const vk::PipelineLayout& getVkPipelineLayout( ) const;
    };
}  // namespace GraphicCore

#endif  // PIPLINE_LAYOUT_HPP
