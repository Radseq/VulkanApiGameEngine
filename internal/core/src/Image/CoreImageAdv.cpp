#include "CoreImageAdv.hpp"

namespace GameCore
{
	void CoreImageAdvance::createImage (const VulkanDevice& device, const uint32_t& Width, const uint32_t& Height, const  vk::SampleCountFlagBits& numSamples, const vk::Format& format, const vk::ImageTiling& tiling, const vk::ImageUsageFlags& usage, const vk::MemoryPropertyFlags& properties) {
		imageFormat = format;

		width = Width;
		height = Height;

		mipLevels = static_cast<uint32_t>(std::floor (std::log2 (GameCore::util::maxFromTwo (Width, Height)))) + 1;

		vk::ImageCreateInfo imageInfo = {};
		imageInfo.imageType = vk::ImageType::e2D;
		imageInfo.extent.width = width;
		imageInfo.extent.height = height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = mipLevels;
		imageInfo.arrayLayers = 1;
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = vk::ImageLayout::eUndefined;
		imageInfo.usage = usage;
		imageInfo.samples = numSamples;
		imageInfo.sharingMode = vk::SharingMode::eExclusive;

		super::createImage (device, imageInfo, properties);

		//super ().createImage (device, imageInfo, properties);
	}

	void CoreImageAdvance::generateMipmaps (const VulkanDevice& device)
	{
		vk::FormatProperties formatProperties = device.getDevice().GetPhysicalDevice().getFormatProperties (imageFormat);

		// Check if image format supports linear blitting
		if (!(formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eSampledImageFilterLinear)) {
			throw std::runtime_error ("texture image format does not support linear blitting!");
		}

		SingleTimeCommandBuffer singleTimeCmdBuffer (device);
		singleTimeCmdBuffer.beginSingleTimeCommands ();

		vk::ImageMemoryBarrier barrier = {};
		barrier.image = super::getVkImage ();
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.subresourceRange.levelCount = 1;

		int32_t mipWidth = static_cast<int32_t>(width);
		int32_t mipHeight = static_cast<int32_t>(height);

		for (uint32_t i = 1; i < mipLevels; i++) {
			barrier.subresourceRange.baseMipLevel = i - 1;
			barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
			barrier.newLayout = vk::ImageLayout::eTransferSrcOptimal;
			barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
			barrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;

			singleTimeCmdBuffer.getCommandBuffer ().pipelineBarrier (vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eTransfer, {}, nullptr, nullptr, barrier);

			vk::ImageBlit blit = {};
			blit.srcOffsets[0] = { 0, 0, 0 };
			blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
			blit.srcSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
			blit.srcSubresource.mipLevel = i - 1;
			blit.srcSubresource.baseArrayLayer = 0;
			blit.srcSubresource.layerCount = 1;
			blit.dstOffsets[0] = { 0, 0, 0 };
			blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
			blit.dstSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
			blit.dstSubresource.mipLevel = i;
			blit.dstSubresource.baseArrayLayer = 0;
			blit.dstSubresource.layerCount = 1;

			singleTimeCmdBuffer.getCommandBuffer ().blitImage (super::getVkImage (), vk::ImageLayout::eTransferSrcOptimal, super::getVkImage (), vk::ImageLayout::eTransferDstOptimal, blit, vk::Filter::eLinear);

			barrier.oldLayout = vk::ImageLayout::eTransferSrcOptimal;
			barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			barrier.srcAccessMask = vk::AccessFlagBits::eTransferRead;
			barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

			singleTimeCmdBuffer.getCommandBuffer ().pipelineBarrier (vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, {}, nullptr, nullptr, barrier);

			if (mipWidth > 1) mipWidth /= 2;
			if (mipHeight > 1) mipHeight /= 2;
		}

		barrier.subresourceRange.baseMipLevel = mipLevels - 1;
		barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
		barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
		barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

		singleTimeCmdBuffer.getCommandBuffer ().pipelineBarrier (vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, {}, nullptr, nullptr, barrier);

		singleTimeCmdBuffer.endSingleTimeCommands ();
	}

	void CoreImageAdvance::transitionImageLayout (const VulkanDevice& device, const vk::ImageLayout& oldLayout, const vk::ImageLayout& newLayout)
	{
		GameCore::SingleTimeCommandBuffer singleTimeCmdBuffer (device);
		singleTimeCmdBuffer.beginSingleTimeCommands ();

		vk::ImageMemoryBarrier barrier = {};
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = super::getVkImage ();
		barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = mipLevels;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		vk::PipelineStageFlags sourceStage;
		vk::PipelineStageFlags destinationStage;

		if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal) {
			barrier.srcAccessMask = vk::AccessFlagBits::eIndirectCommandRead; //??????? should be 0 not 01
			barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

			sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
			destinationStage = vk::PipelineStageFlagBits::eTransfer;
		}
		else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
			barrier.srcAccessMask = vk::AccessFlagBits::eShaderRead;
			barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

			sourceStage = vk::PipelineStageFlagBits::eTransfer;
			destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
		}
		else {
			throw std::invalid_argument ("unsupported layout transition!");
		}

		singleTimeCmdBuffer.getCommandBuffer ().pipelineBarrier (sourceStage, destinationStage, {}, nullptr, nullptr, barrier);
		singleTimeCmdBuffer.endSingleTimeCommands ();
	}

	void CoreImageAdvance::destroy (const vk::Device& logicDevice)
	{
		if (sampler != vk::Sampler())
			logicDevice.destroySampler (sampler);
		super::destroy (logicDevice);
		//super ().destroy (logicDevice);
	}

	void CoreImageAdvance::createSampler (const VulkanDevice& device)
	{
		vk::Bool32 samplerAnisotropy = device.getDevice ().GetPhysicalDeviceFeatures ().samplerAnisotropy;

		vk::SamplerCreateInfo samplerInfo = {};
		samplerInfo.magFilter = vk::Filter::eLinear;
		samplerInfo.minFilter = vk::Filter::eLinear;
		samplerInfo.addressModeU = vk::SamplerAddressMode::eRepeat;
		samplerInfo.addressModeV = vk::SamplerAddressMode::eRepeat;
		samplerInfo.addressModeW = vk::SamplerAddressMode::eRepeat;
		samplerInfo.anisotropyEnable = samplerAnisotropy;
		samplerInfo.maxAnisotropy = samplerAnisotropy ? device.getDevice ().GetDeviceProperties ().limits.maxSamplerAnisotropy : 1.0f;
		samplerInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = vk::CompareOp::eAlways;
		samplerInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
		samplerInfo.minLod = 0;
		samplerInfo.maxLod = static_cast<float>(mipLevels);
		samplerInfo.mipLodBias = 0;

		sampler = device.getVkDevice ().createSampler (samplerInfo);
	}

	const vk::Sampler CoreImageAdvance::getSampler ()
	{
		return sampler;
	}
}
