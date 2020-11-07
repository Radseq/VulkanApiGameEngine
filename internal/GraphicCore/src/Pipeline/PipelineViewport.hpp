#ifndef PIPLINE_VIEWPORT_HPP
#define PIPLINE_VIEWPORT_HPP

#include <Util\util.hpp>
#include <vulkan/vulkan.hpp>

namespace GraphicCore {
    class PipelineViewport {
        vk::PipelineViewportStateCreateInfo viewportState;
        std::vector<vk::Viewport>           viewports;
        std::vector<vk::Rect2D>             scissors;

       public:
        PipelineViewport( )  = default;
        ~PipelineViewport( ) = default;

        void                                 update( );
        vk::PipelineViewportStateCreateInfo& getViewPort( );

        void setViewportAndScissor (float width, float height);
        void setViewportAndScissor (const vk::Extent2D& windowSize);
    };
}  // namespace GraphicCore

#endif PIPLINE_VIEWPORT_HPP
