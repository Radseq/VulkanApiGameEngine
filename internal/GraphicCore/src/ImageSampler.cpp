#include "pch.h"
#include "ImageSampler.hpp"

namespace GraphicCore
{
    void ImageSampler::SetAnisotropy (vk::SamplerCreateInfo&             SamplerInfo,
                                      const GraphicCore::PhysicalDevice& PhysicalDevice)
    {
        vk::Bool32 samplerAnisotropy = PhysicalDevice.GetPhysicalDeviceFeatures( ).samplerAnisotropy;

        SamplerInfo.anisotropyEnable = samplerAnisotropy;
        SamplerInfo.maxAnisotropy =
            samplerAnisotropy ? PhysicalDevice.GetDeviceProperties( ).limits.maxSamplerAnisotropy : 1.0F;
    }

    ImageSampler::ImageSampler (const vk::Device& Device)
        : m_Device (Device)
    {
    }

    void ImageSampler::Create (const vk::SamplerCreateInfo& CreateInfo)
    {
        m_Sampler = m_Device.createSampler (CreateInfo);
    }

    void ImageSampler::Create (const GraphicCore::CoreImage& image, const GraphicCore::PhysicalDevice& PhysicalDevice)
    {
        vk::SamplerCreateInfo samplerInfo = { };
        samplerInfo.minFilter = samplerInfo.magFilter = vk::Filter::eLinear;
        samplerInfo.addressModeU                      = vk::SamplerAddressMode::eRepeat;
        samplerInfo.addressModeV                      = vk::SamplerAddressMode::eRepeat;
        samplerInfo.addressModeW                      = vk::SamplerAddressMode::eRepeat;

        SetAnisotropy (samplerInfo, PhysicalDevice);

        samplerInfo.borderColor             = vk::BorderColor::eFloatOpaqueWhite;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable           = VK_FALSE;
        samplerInfo.compareOp               = vk::CompareOp::eNever;
        samplerInfo.mipmapMode              = vk::SamplerMipmapMode::eLinear;
        samplerInfo.minLod                  = 0;
        samplerInfo.maxLod                  = static_cast<float> (image.GetMipLevels( ));
        samplerInfo.mipLodBias              = 0;

        Create (samplerInfo);
    }

    ImageSampler::ImageSampler (ImageSampler&& otherSampler)
        : m_Device {otherSampler.m_Device}
        , m_Sampler {otherSampler.m_Sampler}
    {
        otherSampler.m_Sampler = nullptr;
    }

    ImageSampler::~ImageSampler( )
    {
        if (m_Sampler != vk::Sampler( )) m_Device.destroySampler (m_Sampler);
    }

    const vk::Sampler& ImageSampler::GetVkSampler( ) const { return m_Sampler; }
}  // namespace GraphicCore
