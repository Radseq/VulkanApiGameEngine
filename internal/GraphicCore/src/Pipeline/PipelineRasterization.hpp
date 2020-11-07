#ifndef PIPLINE_RESTERIZATION_HPP
#define PIPLINE_RESTERIZATION_HPP

#include <vulkan/vulkan.hpp>

namespace GraphicCore {
    class PipelineRasterization {
        vk::PipelineRasterizationStateCreateInfo rasterizationState;

       public:
        PipelineRasterization( );
        ~PipelineRasterization( ) = default;
        vk::PipelineRasterizationStateCreateInfo& getRasterizationState( );
    };
}  // namespace GraphicCore

#endif PIPLINE_RESTERIZATION_HPP
