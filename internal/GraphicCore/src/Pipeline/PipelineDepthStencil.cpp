#include "PipelineDepthStencil.hpp"

namespace GraphicCore {
    PipelineDepthStencil::PipelineDepthStencil (bool depthEnable) {
        if (depthEnable) {
            depthStencilState.depthTestEnable  = VK_TRUE;
            depthStencilState.depthWriteEnable = VK_TRUE;
            depthStencilState.depthCompareOp   = vk::CompareOp::eLessOrEqual;
        }
    }

    vk::PipelineDepthStencilStateCreateInfo& PipelineDepthStencil::getDepthStencil( ) { return depthStencilState; }
}  // namespace GraphicCore
