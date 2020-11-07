#include "PipelineLayout.hpp"
namespace GraphicCore
{
    void PipelineLayout::setPushConstant (const vk::PushConstantRange& pushConstantRange)
    {
        util::updatePointerFromData (pushConstantRange, pipelineLayoutInfo.pushConstantRangeCount,
                                     pipelineLayoutInfo.pPushConstantRanges);
    }

    void PipelineLayout::setPushConstant (const std::vector<vk::PushConstantRange>& pushConstantRange)
    {
        util::updatePointerFromVector (pushConstantRange, pipelineLayoutInfo.pushConstantRangeCount,
                                       pipelineLayoutInfo.pPushConstantRanges);
    }

    void PipelineLayout::create (const vk::Device& device, const GraphicCore::DescriptorSetLayoutBinding& descSetLayout)
    {
        util::updatePointerFromData (descSetLayout.getDescriptorSetLayout( ), pipelineLayoutInfo.setLayoutCount,
                                     pipelineLayoutInfo.pSetLayouts);

        pipelineLayout = device.createPipelineLayout (pipelineLayoutInfo);
    }

    const vk::PipelineLayout& PipelineLayout::getVkPipelineLayout( ) const { return pipelineLayout; }
}  // namespace GraphicCore