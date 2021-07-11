#include "pch.h"
#include "PipelineViewport.hpp"

namespace GraphicCore
{
    void PipelineViewport::update( )
    {
        if (viewports.empty( ))
        {
            viewportState.viewportCount = 1;
            viewportState.pViewports    = nullptr;
        }
        else
        {
            Util::updatePointerFromVector (viewports, viewportState.viewportCount, viewportState.pViewports);
        }

        if (scissors.empty( ))
        {
            viewportState.scissorCount = 1;
            viewportState.pScissors    = nullptr;
        }
        else
        {
            Util::updatePointerFromVector (scissors, viewportState.scissorCount, viewportState.pScissors);
        }
    }

    vk::PipelineViewportStateCreateInfo& PipelineViewport::getViewPort( ) { return viewportState; }

    void PipelineViewport::setViewportAndScissor (float width, float height)
    {
        const vk::Viewport viewPort {0, 0, width, height, 0, 1.0f};
        viewports.push_back (viewPort);

        const vk::Rect2D scissor {{ }, {uint32_t (width), uint32_t (height)}};
        scissors.push_back (scissor);
    }

    void PipelineViewport::setViewportAndScissor (const vk::Extent2D& windowSize)
    {
        const vk::Viewport viewPort {
            0, 0, static_cast<float> (windowSize.width), static_cast<float> (windowSize.height), 0, 1.0f};
        viewports.push_back (viewPort);

        const vk::Rect2D scissor {{ }, {windowSize.width, windowSize.height}};
        scissors.push_back (scissor);
    }
}  // namespace GraphicCore
