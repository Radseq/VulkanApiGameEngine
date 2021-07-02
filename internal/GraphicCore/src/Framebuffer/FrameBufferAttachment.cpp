#include "FrameBufferAttachment.hpp"

namespace GraphicCore {
	
	bool FrameBufferAttachment::HasDepth()
	{
		static std::vector<VkFormat> formats =
		{
			VK_FORMAT_D16_UNORM,
			VK_FORMAT_X8_D24_UNORM_PACK32,
			VK_FORMAT_D32_SFLOAT,
			VK_FORMAT_D16_UNORM_S8_UINT,
			VK_FORMAT_D24_UNORM_S8_UINT,
			VK_FORMAT_D32_SFLOAT_S8_UINT,
		};
		return std::find(formats.begin(), formats.end(), m_Format) != std::end(formats);
	}

	bool FrameBufferAttachment::HasStencil()
	{
		static std::vector<VkFormat> formats =
		{
			VK_FORMAT_S8_UINT,
			VK_FORMAT_D16_UNORM_S8_UINT,
			VK_FORMAT_D24_UNORM_S8_UINT,
			VK_FORMAT_D32_SFLOAT_S8_UINT,
		};
		return std::find(formats.begin(), formats.end(), m_Format) != std::end(formats);
	}

	bool FrameBufferAttachment::IsDepthStencil()
	{
		return(HasDepth() || HasStencil());
	}
   
}  // namespace GraphicCore
