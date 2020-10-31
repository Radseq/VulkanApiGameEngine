#include "PipelineRasterization.hpp"

namespace GameCore {
    PipelineRasterization::PipelineRasterization( ) {
        rasterizationState.lineWidth = 1.0F;
        rasterizationState.cullMode  = vk::CullModeFlagBits::eBack;
    }

    vk::PipelineRasterizationStateCreateInfo& PipelineRasterization::getRasterizationState( ) {
        return rasterizationState;
    }
}  // namespace GameCore
