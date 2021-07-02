#ifndef PIPLINE_DYNAMIC_HPP
#define PIPLINE_DYNAMIC_HPP

#include "pch.h"
#include "CoreUtil.hpp"

namespace GraphicCore
{
    class PipelineDynamic
    {
        std::vector<vk::DynamicState>      dynamicStateEnables;
        vk::PipelineDynamicStateCreateInfo dynamicState;

       public:
        PipelineDynamic (bool useDynamic = false);
        ~PipelineDynamic( ) = default;
        void                                update( );
        vk::PipelineDynamicStateCreateInfo& getDynamicState( );
        std::vector<vk::DynamicState>&      getDynamicStateEnables( );
    };
}  // namespace GraphicCore

#endif  // PIPLINE_DYNAMIC_HPP
