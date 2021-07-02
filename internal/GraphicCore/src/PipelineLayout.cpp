#include "pch.h"
#include "PipelineLayout.hpp"

namespace GraphicCore
{
    void PipelineLayout::setPushConstant (const vk::PushConstantRange& pushConstantRange)
    {
        assert (sizeof (pushConstantRange) <= m_Device.getDevice( ).GetDeviceProperties( ).limits.maxPushConstantsSize);

        Util::updatePointerFromData (pushConstantRange, pipelineLayoutInfo.pushConstantRangeCount,
                                     pipelineLayoutInfo.pPushConstantRanges);
    }

    void PipelineLayout::setPushConstant (const std::vector<vk::PushConstantRange>& pushConstantRange)
    {
        assert (sizeof (pushConstantRange) <= m_Device.getDevice( ).GetDeviceProperties( ).limits.maxPushConstantsSize);

        Util::updatePointerFromVector (pushConstantRange, pipelineLayoutInfo.pushConstantRangeCount,
                                       pipelineLayoutInfo.pPushConstantRanges);
    }

    void PipelineLayout::create (const GraphicCore::DescriptorSetLayoutBinding& descSetLayout)
    {
        Util::updatePointerFromData (descSetLayout.getDescriptorSetLayout( ), pipelineLayoutInfo.setLayoutCount,
                                     pipelineLayoutInfo.pSetLayouts);

        pipelineLayout = m_Device.getVkDevice( ).createPipelineLayout (pipelineLayoutInfo);
    }

    const vk::PipelineLayout& PipelineLayout::getVkPipelineLayout( ) const { return pipelineLayout; }
}  // namespace GraphicCore