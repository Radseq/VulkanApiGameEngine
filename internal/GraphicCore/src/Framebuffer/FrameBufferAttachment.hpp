#ifndef FRAMEBUFFER_ATTACHMENT_HPP
#define FRAMEBUFFER_ATTACHMENT_HPP

#include <vulkan/vulkan.hpp>


namespace GraphicCore
{
	//image
	//imageview

    class FrameBufferAttachment
    {

       public:
		bool HasDepth();
		bool HasStencil();
		bool IsDepthStencil();
    };
}  // namespace GraphicCore

#endif FRAMEBUFFER_ATTACHMENT_HPP
