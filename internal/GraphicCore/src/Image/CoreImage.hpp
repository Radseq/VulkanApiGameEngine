#ifndef CORE_IMAGE_HPP
#define CORE_IMAGE_HPP

#include <vulkan/vulkan.hpp>
#include <Device\VulkanDevice.hpp>
#include <Command\SingleTimeCommandBuffer.hpp>
#include "IGetImage.hpp"

namespace GraphicCore
{
	class CoreImage : public IGetImage
	{
		void allocateMemory		(const VulkanDevice& device, const vk::MemoryPropertyFlags& properties);

		vk::Image			image;

	protected:
		vk::DeviceMemory	imageMemory	{nullptr};
		vk::Format			imageFormat{ vk::Format::eUndefined };

		uint32_t			mipLevels	{ 1 };
		uint32_t			width		{ 0 };
		uint32_t			height		{ 0 };
		
	public:
		void createImage		(const VulkanDevice& device, const vk::ImageCreateInfo& imageCreateInfo, const vk::MemoryPropertyFlags& properties);
		void destroy			(const vk::Device& logicDevice);

		const vk::Image& getVkImage ();
		const vk::Format& getFormat ();
		const uint32_t& getMipLevels ();
	};
}
#endif CORE_IMAGE_HPP
