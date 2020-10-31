#ifndef PIPLINE_COLOR_BLEND_ATT_HPP
#define PIPLINE_COLOR_BLEND_ATT_HPP

#include <vulkan/vulkan.hpp>

#include "../Util/VKHelpers.hpp"

namespace GameCore {
    class PipelineColorBlendAttachment : public vk::PipelineColorBlendAttachmentState {
        using Parent = PipelineColorBlendAttachmentState;
        // vk::PipelineColorBlendAttachmentState colorBlendAttachmentState;
       public:
        PipelineColorBlendAttachment (vk::Bool32                     blendEnable_         = 0,
                                      vk::BlendFactor                srcColorBlendFactor_ = vk::BlendFactor::eZero,
                                      vk::BlendFactor                dstColorBlendFactor_ = vk::BlendFactor::eZero,
                                      vk::BlendOp                    colorBlendOp_        = vk::BlendOp::eAdd,
                                      vk::BlendFactor                srcAlphaBlendFactor_ = vk::BlendFactor::eZero,
                                      vk::BlendFactor                dstAlphaBlendFactor_ = vk::BlendFactor::eZero,
                                      vk::BlendOp                    alphaBlendOp_        = vk::BlendOp::eAdd,
                                      const vk::ColorComponentFlags& colorWriteMask_ = vkHelper::fullColorWriteMask( ));

        ~PipelineColorBlendAttachment( ) = default;
        // vk::PipelineColorBlendAttachmentState & returnColorBlendAttachment();
    };
}  // namespace GameCore

#endif PIPLINE_COLOR_BLEND_ATT_HPP
