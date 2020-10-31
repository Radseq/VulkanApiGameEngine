#ifndef I_GET_IMAGE_HPP
#define I_GET_IMAGE_HPP

#include <vulkan/vulkan.hpp>

namespace GameCore
{
	class IGetImage
	{
	public:
		virtual const vk::Image& getVkImage () = 0;
		virtual const vk::Format& getFormat () = 0;
		virtual const uint32_t& getMipLevels () = 0;
	};
}
#endif I_GET_IMAGE_HPP
