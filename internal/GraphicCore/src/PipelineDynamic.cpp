#include "pch.h"
#include "PipelineDynamic.hpp"

namespace GraphicCore
{
    PipelineDynamic::PipelineDynamic (bool useDynamic)
    {
        if (useDynamic) { dynamicStateEnables = {vk::DynamicState::eViewport, vk::DynamicState::eScissor}; }
    }

    void PipelineDynamic::update( )
    {
        Util::updatePointerFromVector (dynamicStateEnables, dynamicState.dynamicStateCount,
                                       dynamicState.pDynamicStates);
    }

    vk::PipelineDynamicStateCreateInfo& PipelineDynamic::getDynamicState( ) { return dynamicState; }
    std::vector<vk::DynamicState>&      PipelineDynamic::getDynamicStateEnables( ) { return dynamicStateEnables; }
}  // namespace GraphicCore
