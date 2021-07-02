#include "pch.h"
#include "CoreImageView.hpp"

namespace GraphicCore
{
    CoreImageView::CoreImageView (const vk::Device& Device)
        : m_Device (Device)
    {
    }

    void CoreImageView::Destroy( ) { m_Device.destroyImageView (m_ImageView); }

    void CoreImageView::SetImage (std::shared_ptr<CoreImage> relatedImage) { m_Image = relatedImage; }

    const vk::ImageSubresourceRange& CoreImageView::getSubresourceRange( ) { return m_SubRange; }
    const vk::ImageSubresourceLayers& CoreImageView::getSubresourceLayers( ) { return m_SubResLayers; }


    vk::ImageAspectFlags CoreImageView::SetAspectMaskByFormat (const vk::Format& format)
    {
        vk::ImageAspectFlags flag {vk::ImageAspectFlagBits::eColor};

        if (vkHelper::HasDepthFormat (format)) { flag = vk::ImageAspectFlagBits::eDepth; }
        else if (vkHelper::HasDepthStencilFormat (format))
        {
            flag = vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
        }
        // else
        //{
        //    flag = vk::ImageAspectFlagBits::eColor;
        //}
        return flag;
    }


    void CoreImageView::CreateImageView (std::shared_ptr<CoreImage> Image, const vk::ImageViewType& viewType,
                                         const vk::Format& Format)
    {
        if (Format == vk::Format::eUndefined) m_Format = Image->GetFormat( );

        vk::ImageViewCreateInfo viewInfo = { };
        viewInfo.image                   = Image->GetVkImage( );
        viewInfo.viewType                = viewType;
        viewInfo.format                  = m_Format;
        SetSubResRange (Image);
        SetSubResLayers( );

        viewInfo.subresourceRange = m_SubRange;

        m_ImageView = m_Device.createImageView (viewInfo);

        m_Image = Image;
    }

    void CoreImageView::SetSubResRange (std::shared_ptr<GraphicCore::CoreImage> Image)
    {
        m_SubRange.aspectMask     = SetAspectMaskByFormat (m_Format);
        m_SubRange.baseMipLevel   = 0;
        m_SubRange.levelCount     = Image->GetMipLevels( );
        m_SubRange.baseArrayLayer = 0;
        m_SubRange.layerCount     = Image->GetLayerCount( );
    }

    void CoreImageView::SetSubResLayers( )
    {
        m_SubResLayers.aspectMask     = m_SubRange.aspectMask;
        m_SubResLayers.baseArrayLayer = m_SubRange.baseArrayLayer;
        m_SubResLayers.layerCount     = m_SubRange.layerCount;
        m_SubResLayers.mipLevel       = m_SubRange.baseMipLevel;
    }

    const vk::ImageView&       CoreImageView::GetImageView( ) { return m_ImageView; }
    std::shared_ptr<CoreImage> CoreImageView::GetCoreImage( ) const { return m_Image; }

}  // namespace GraphicCore
