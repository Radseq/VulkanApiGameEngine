#include "CoreImage.hpp"

namespace GameCore
{
	void CoreImage::allocateMemory (const VulkanDevice& device, const vk::MemoryPropertyFlags& properties)
	{
		const vk::MemoryRequirements memRequirements = device.getVkDevice().getImageMemoryRequirements (image);

		vk::MemoryAllocateInfo allocInfo = {};
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = device.getMemoryType (memRequirements.memoryTypeBits, properties);


		imageMemory = device.getVkDevice().allocateMemory (allocInfo);

		device.getVkDevice ().bindImageMemory (image, imageMemory, 0);






		/*
		GameCore::AllocationCreateInfo allocInfo{};
		allocInfo.size = memRequirements.size;
		// Get the memory type index that supports host visible memory access
		// Most implementations offer multiple memory types and selecting the
		// correct one to allocate memory from is important
		allocInfo.memoryTypeIndex = context->getMemoryType (memRequirements.memoryTypeBits, properties);
		allocInfo.usage = properties;

		GameCore::AllocationProperties allocatedMemory{};

		context->getAllocator ()->alloc (allocatedMemory, allocInfo);
		context->getVkDevice ().bindImageMemory (image, allocatedMemory.handle, allocatedMemory.offset);*/
	}

	void CoreImage::createImage (const VulkanDevice& device, const vk::ImageCreateInfo& imageCreateInfo, const vk::MemoryPropertyFlags& properties)
	{
		imageFormat = imageCreateInfo.format;

		width = imageCreateInfo.extent.width;
		height = imageCreateInfo.extent.height;
		mipLevels = imageCreateInfo.mipLevels;

		image = device.getVkDevice().createImage (imageCreateInfo);

		allocateMemory (device, properties);
	}

	
	void CoreImage::destroy (const vk::Device& logicDevice)
	{
		logicDevice.freeMemory (imageMemory);
		logicDevice.destroyImage (image);
	}

	const vk::Image& CoreImage::getVkImage ()
	{
		return image;
	}

	const vk::Format& CoreImage::getFormat ()
	{
		return imageFormat;
	}
	const uint32_t& CoreImage::getMipLevels ()
	{
		return mipLevels;
	}
}
