#ifndef CORE_IMAGE_VIEW_HPP
#define CORE_IMAGE_VIEW_HPP

#include <vulkan/vulkan.hpp>
#include <Image\IGetImage.hpp>

namespace GameCore
{
	class CoreImageView
	{
		vk::Device localDevice;
		vk::ImageView colorImageView;
	public:
		void destroy (const vk::Device localDevice);
		void createImageView (const vk::Device localDevice, IGetImage& getImage, const vk::ImageAspectFlags& aspectFlags);
		const vk::ImageView& getImageView ();
	};
}
#endif CORE_IMAGE_VIEW_HPP
