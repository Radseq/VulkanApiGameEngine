#ifndef CORE_IMAGE_ADVANCED_HPP
#define CORE_IMAGE_ADVANCED_HPP

#include <vulkan/vulkan.hpp>
#include <Device\VulkanDevice.hpp>
#include <Command\SingleTimeCommandBuffer.hpp>
#include "CoreImage.hpp"
#include "IGetImage.hpp"

namespace GraphicCore
{
	class CoreImageAdvance :public CoreImage
	{
		typedef CoreImage	super; // when use super().create image after exit merthod vk_image is null why? cuse use CoreImage.create image works fine....

		vk::Sampler			sampler;

	public:
		void createImage (const VulkanDevice& device, const uint32_t& width, const uint32_t& height, const vk::SampleCountFlagBits& numSamples, const vk::Format& format, const vk::ImageTiling& tiling, const vk::ImageUsageFlags& usage, const vk::MemoryPropertyFlags& properties);
		void generateMipmaps (const VulkanDevice& device);
		void transitionImageLayout (const VulkanDevice& device, const vk::ImageLayout& oldLayout, const vk::ImageLayout& newLayout);
		void destroy (const vk::Device& logicDevice);
		void createSampler (const VulkanDevice& device);

		const vk::Sampler getSampler ();
	};
}
#endif CORE_IMAGE_ADVANCED_HPP
