#include "PipelineRasterization.hpp"

namespace GraphicCore {
    PipelineRasterization::PipelineRasterization( ) {
        rasterizationState.lineWidth = 1.0F;
        rasterizationState.cullMode  = vk::CullModeFlagBits::eBack;
    }

    vk::PipelineRasterizationStateCreateInfo& PipelineRasterization::getRasterizationState( ) {
        return rasterizationState;
    }
}  // namespace GraphicCore
