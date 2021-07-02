#ifndef IMAGE_SAMPLER_HPP
#define IMAGE_SAMPLER_HPP

#include "CoreImage.hpp"
#include "PhysicalDevice.hpp"
#include "pch.h"

namespace GraphicCore
{
    class ImageSampler
    {
        const vk::Device &m_Device;
        vk::Sampler       m_Sampler {nullptr};

        void SetAnisotropy (vk::SamplerCreateInfo &SamplerInfo, const GraphicCore::PhysicalDevice &PhysicalDevice);

       public:
        ImageSampler (const vk::Device &Device);

        void Create (const vk::SamplerCreateInfo &CreateInfo);
        void Create (const GraphicCore::CoreImage &image, const GraphicCore::PhysicalDevice &PhysicalDevice);

        ImageSampler (const ImageSampler &) = delete;
        ImageSampler (ImageSampler &&otherSampler);
        ImageSampler &operator= (const ImageSampler &) = delete;
        ImageSampler &operator= (ImageSampler &&) = delete;

        ~ImageSampler( );

        const vk::Sampler &GetVkSampler( ) const;
    };
}  // namespace GraphicCore

#endif  // IMAGE_SAMPLER_HPP
