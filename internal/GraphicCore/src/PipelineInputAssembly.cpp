#include "pch.h"
#include "PipelineInputAssembly.hpp"

namespace GraphicCore
{
    PipelineInputAssembly::PipelineInputAssembly( )
    {
        inputAssemblyState.topology = vk::PrimitiveTopology::eTriangleList;
    }

    vk::PipelineInputAssemblyStateCreateInfo& PipelineInputAssembly::getAssembly( ) { return inputAssemblyState; }
}  // namespace GraphicCore
