#ifndef PIPLINE_DEPTH_STENCIL_HPP
#define PIPLINE_DEPTH_STENCIL_HPP

#include <vulkan/vulkan.hpp>

namespace GraphicCore {
    class PipelineDepthStencil {
        vk::PipelineDepthStencilStateCreateInfo depthStencilState;

       public:
        PipelineDepthStencil (bool depthEnable = false);
        ~PipelineDepthStencil( ) = default;
        vk::PipelineDepthStencilStateCreateInfo& getDepthStencil( );
    };
}  // namespace GraphicCore

#endif PIPLINE_DEPTH_STENCIL_HPP
