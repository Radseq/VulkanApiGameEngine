#include "CoreImageView.hpp"

namespace GameCore
{
	void CoreImageView::destroy (const vk::Device localDevice)
	{
		localDevice.destroyImageView (colorImageView);
	}
	
	void CoreImageView::createImageView (const vk::Device localDevice, IGetImage& getImage, const vk::ImageAspectFlags& aspectFlags)
	{
		vk::ImageViewCreateInfo viewInfo = {};
		viewInfo.image = getImage.getVkImage();
		viewInfo.viewType = vk::ImageViewType::e2D;
		viewInfo.format = getImage.getFormat();
		viewInfo.subresourceRange.aspectMask = aspectFlags;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = getImage.getMipLevels();
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		colorImageView = localDevice.createImageView (viewInfo);
	}

	const vk::ImageView& CoreImageView::getImageView () 
	{
		return colorImageView;
	}
}
