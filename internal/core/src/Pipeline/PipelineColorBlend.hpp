#ifndef PIPLINE_COLOR_BLEND_HPP
#define PIPLINE_COLOR_BLEND_HPP

#include <Util\util.hpp>
#include <vector>
#include <vulkan/vulkan.hpp>

#include "../../src/Util/utils.hpp"
#include "PipelineColorBlendAttachment.hpp"

namespace GameCore {
    class PipelineColorBlend : public vk::PipelineColorBlendStateCreateInfo {
        // PipelineColorBlendAttachment pipelineColorBlendAttachment{};
        // Default to a single color attachment state with no blending
        std::vector<vk::PipelineColorBlendAttachmentState> blendAttachmentStates { };

        // vk::PipelineColorBlendStateCreateInfo colorBlendState;
       public:
        PipelineColorBlend( )  = default;
        ~PipelineColorBlend( ) = default;

        void update( );
        // vk::PipelineColorBlendStateCreateInfo & getColorBlend();
        std::vector<vk::PipelineColorBlendAttachmentState>& getBlendAttachmentStates( );
    };
}  // namespace GameCore

#endif PIPLINE_COLOR_BLEND_HPP
