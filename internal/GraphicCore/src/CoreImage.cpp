#include "pch.h"
#include "CoreImage.hpp"

namespace GraphicCore
{
    void CoreImage::SetImageType( )
    {
        uint8_t dim_num {0};

        if (m_Extent.width >= 1) { ++dim_num; }

        if (m_Extent.height >= 1) { ++dim_num; }

        if (m_Extent.depth > 1) { ++dim_num; }

        switch (dim_num)
        {
            case 1:
                m_Type = vk::ImageType::e1D;
                break;
            case 2:
                m_Type = vk::ImageType::e2D;
                break;
            case 3:
                m_Type = vk::ImageType::e3D;
                break;
            default:
                throw std::runtime_error ("No image type found.");
                break;
        }
    }

    CoreImage::CoreImage (const vk::Device& Device, const vk::Image& image, vk::Extent3D& Extent,
                          const vk::Format& Format, const vk::ImageUsageFlags& UsageFlags)
        : m_Device (Device)
        , m_Image (image)
        , m_Extent (Extent)
        , m_Format (Format)
        , m_UsageFlags (UsageFlags)
    {
        m_Subresource.mipLevel   = 1;
        m_Subresource.arrayLayer = 1;

        SetImageType( );  // will be probelm if this func throw error
    }

    CoreImage::CoreImage (const vk::Device& Device, const vk::Extent3D& Extent, const vk::Format& Format,
                          const vk::ImageUsageFlags& UsageFlags, const vk::MemoryPropertyFlags& MemoryProperties,
                          const vk::SampleCountFlagBits SampleCount, const uint32_t& MipLevels,
                          const uint32_t& ArrayLayers, const vk::ImageTiling& Tiling)
        : m_Device (Device)
        , m_Extent (Extent)
        , m_Format (Format)
        , m_UsageFlags (UsageFlags)
        , m_MemoryProperties (MemoryProperties)
        , m_SampleCount (SampleCount)
        , m_MipLevels (MipLevels)
        , m_ArrayLayer (ArrayLayers)
        , m_Tiling (Tiling)
    {
        assert (MipLevels > 0 && "Image should have at least one level");
        assert (ArrayLayers > 0 && "Image should have at least one layer");

        m_Subresource.mipLevel   = MipLevels;
        m_Subresource.arrayLayer = ArrayLayers;

        SetImageType( );  // will be probelm if this func throw error
    }

    void CoreImage::destroy( )
    {
        if (m_Device != vk::Device( ))
        {
            if (m_Image)
            {
                m_Device.destroyImage (m_Image);
                m_Image = vk::Image( );
            }

            // logicDevice.getAllocator ()->free (allocatedMemory); // todo:(
        }
    }

    const vk::Extent3D& CoreImage::GetExtend( ) const { return m_Extent; }
    const vk::Format&   CoreImage::GetFormat( ) const { return m_Format; }
    const vk::Device&   CoreImage::GetDevice( ) const { return m_Device; }

    const vk::SampleCountFlagBits& CoreImage::GetSamplesFlag( ) const { return m_SampleCount; }
    const vk::ImageType&           CoreImage::GetType( ) const { return m_Type; }
    const vk::Image&               CoreImage::GetVkImage( ) const { return m_Image; }
    const vk::ImageUsageFlags&     CoreImage::GetUsageFlags( ) const { return m_UsageFlags; }
    const uint32_t&                CoreImage::GetMipLevels( ) const { return m_MipLevels; }
    const uint32_t&                CoreImage::GetLayerCount( ) const { return m_ArrayLayer; }

    std::unordered_set<std::shared_ptr<CoreImageView>>& CoreImage::GetViews( ) { return m_Views; }

    void CoreImage::bind( ) const
    {
        m_Device.bindImageMemory (m_Image, allocatedMemory.handle, allocatedMemory.offset);
    }

    void CoreImage::copy (const size_t& size, const void* data, const vk::DeviceSize& offset) const
    {
        memcpy (static_cast<uint8_t*> (mapped) + offset, data, size);
    }

    void CoreImage::unMap( ) { m_Device.unmapMemory (allocatedMemory.handle); }

    void CoreImage::SetMipLevels (const uint32_t& levels) { m_MipLevels = levels; }
    void CoreImage::SetArrayLayer (const uint32_t& ArrayLayer) { m_ArrayLayer = ArrayLayer; }

}  // namespace GraphicCore
