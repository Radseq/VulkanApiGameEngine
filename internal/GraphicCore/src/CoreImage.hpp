#ifndef CORE_IMAGE_HPP
#define CORE_IMAGE_HPP

//#include <CoreImageView.hpp>
#include <unordered_set>

#include "AllocationProperties.hpp"

namespace GraphicCore
{
    class CoreImageView;  // forward declaration, because image depend on image view and visa versa

    class CoreImage
    {
        const vk::Device&             m_Device;
        const vk::Extent3D&           m_Extent;
        const vk::Format&             m_Format;
        const vk::ImageUsageFlags&    m_UsageFlags;
        const vk::MemoryPropertyFlags m_MemoryProperties;

        void* mapped {nullptr};

        vk::SampleCountFlagBits m_SampleCount {vk::SampleCountFlagBits::e1};
        vk::ImageType           m_Type {vk::ImageType::e2D};
        vk::ImageTiling         m_Tiling = vk::ImageTiling::eOptimal;

        // vk::Image m_Image {nullptr};

        std::unordered_set<std::shared_ptr<CoreImageView>> m_Views;

        uint32_t m_MipLevels {1};
        uint32_t m_ArrayLayer {1};

        vk::ImageSubresource       m_Subresource;

        void SetImageType( );

       public:
        CoreImage (const vk::Device& Device, const vk::Image& image, vk::Extent3D& Extent, const vk::Format& Format,
                   const vk::ImageUsageFlags& UsageFlags);

        CoreImage (const vk::Device& Device, const vk::Extent3D& Extent, const vk::Format& Format,
                   const vk::ImageUsageFlags& UsageFlags, const vk::MemoryPropertyFlags& MemoryProperties,
                   const vk::SampleCountFlagBits SampleCount = vk::SampleCountFlagBits::e1,
                   const uint32_t& MipLevels = 1, const uint32_t& ArrayLayers = 1,
                   const vk::ImageTiling& Tiling = vk::ImageTiling::eOptimal);

        ~CoreImage () {
            int a = 10;
        }

        AllocationProperties allocatedMemory { };
        vk::Image            m_Image {nullptr};

        operator bool( ) const { return m_Image.operator bool( ); }

        void destroy( );
        void bind( ) const;

        std::unordered_set<std::shared_ptr<CoreImageView>>& GetViews( );

        template <typename T = void> T* map (vk::DeviceSize size = VK_WHOLE_SIZE)
        {
            mapped = m_Device.mapMemory (allocatedMemory.handle, allocatedMemory.offset, size, vk::MemoryMapFlags( ));
            return (T*) mapped;
        }

        void unMap( );
        void copy (const size_t& size, const void* data, const vk::DeviceSize& offset) const;

        template <typename T> void copy (const T& data, vk::DeviceSize offset = 0) const
        {
            copy (sizeof (T), &data, offset);
        }

        template <typename T> void copy (const std::vector<T>& data, vk::DeviceSize offset = 0) const
        {
            copy (sizeof (T) * data.size( ), data.data( ), offset);
        }

        void SetMipLevels (const uint32_t& levels);
        void SetArrayLayer (const uint32_t& ArrayLayer);

        const vk::Extent3D&            GetExtend( ) const;
        const vk::Format&              GetFormat( ) const;
        const vk::Device&              GetDevice( ) const;
        const vk::SampleCountFlagBits& GetSamplesFlag( ) const;
        const vk::ImageUsageFlags&     GetUsageFlags( ) const;
        const vk::ImageType&           GetType( ) const;
        const vk::Image&               GetVkImage( ) const;
        const uint32_t&                GetMipLevels( ) const;
        const uint32_t&                GetLayerCount( ) const;
    };
}  // namespace GraphicCore

#endif /* CORE_IMAGE_HPP */
