#ifndef PIPLINE_INPUT_ASSEMBLY_HPP
#define PIPLINE_INPUT_ASSEMBLY_HPP

#include "pch.h"

namespace GraphicCore
{
    class PipelineInputAssembly
    {
        vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState;

       public:
        PipelineInputAssembly( );
        ~PipelineInputAssembly( ) = default;
        vk::PipelineInputAssemblyStateCreateInfo& getAssembly( );
    };
}  // namespace GraphicCore

#endif PIPLINE_INPUT_ASSEMBLY_HPP
