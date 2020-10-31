#ifndef PIPLINE_RESTERIZATION_HPP
#define PIPLINE_RESTERIZATION_HPP

#include <vulkan/vulkan.hpp>

namespace GameCore {
    class PipelineRasterization {
        vk::PipelineRasterizationStateCreateInfo rasterizationState;

       public:
        PipelineRasterization( );
        ~PipelineRasterization( ) = default;
        vk::PipelineRasterizationStateCreateInfo& getRasterizationState( );
    };
}  // namespace GameCore

#endif PIPLINE_RESTERIZATION_HPP
