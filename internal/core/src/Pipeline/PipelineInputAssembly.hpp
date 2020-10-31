#ifndef PIPLINE_INPUT_ASSEMBLY_HPP
#define PIPLINE_INPUT_ASSEMBLY_HPP

#include <vulkan/vulkan.hpp>

namespace GameCore {
    class PipelineInputAssembly {
        vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState;

       public:
        PipelineInputAssembly( );
        ~PipelineInputAssembly( ) = default;
        vk::PipelineInputAssemblyStateCreateInfo& getAssembly( );
    };
}  // namespace GameCore

#endif PIPLINE_INPUT_ASSEMBLY_HPP
