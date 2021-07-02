#ifndef CORE_IMAGE_VIEW_HPP
#define CORE_IMAGE_VIEW_HPP

#include "CoreImage.hpp"
#include "pch.h"

namespace GraphicCore
{
    class CoreImage;

    class CoreImageView
    {
        const vk::Device& m_Device;

        vk::ImageView m_ImageView;
        vk::Format    m_Format {vk::Format::eUndefined};

        std::shared_ptr<CoreImage> m_Image;

        vk::ImageSubresourceRange  m_SubRange;
        vk::ImageSubresourceLayers m_SubResLayers;

        void SetSubResRange (std::shared_ptr<GraphicCore::CoreImage> Image);

        void                 SetSubResLayers( );
        vk::ImageAspectFlags SetAspectMaskByFormat (const vk::Format& format);

       public:
        CoreImageView (const vk::Device& Device);

        void Destroy( );
        void CreateImageView (std::shared_ptr<CoreImage> Image, const vk::ImageViewType& viewType,
                              const vk::Format& Format = vk::Format::eUndefined);

        const vk::ImageView& GetImageView( );

        std::shared_ptr<CoreImage> GetCoreImage( ) const;

        void SetImage (std::shared_ptr<CoreImage> RelatedImage);

        const vk::ImageSubresourceRange&  getSubresourceRange( );
        const vk::ImageSubresourceLayers& getSubresourceLayers( );
    };
}  // namespace GraphicCore
#endif CORE_IMAGE_VIEW_HPP
