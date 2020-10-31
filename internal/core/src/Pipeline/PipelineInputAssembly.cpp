#include "PipelineInputAssembly.hpp"

namespace GameCore {
    PipelineInputAssembly::PipelineInputAssembly( ) {
        inputAssemblyState.topology = vk::PrimitiveTopology::eTriangleList;
    }

    vk::PipelineInputAssemblyStateCreateInfo& PipelineInputAssembly::getAssembly( ) { return inputAssemblyState; }
}  // namespace GameCore
