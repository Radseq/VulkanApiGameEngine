#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <vulkan/vulkan.hpp>

#include "Allocation/AllocationProperties.hpp"

namespace GraphicCore
{
    class Image
    {
        void* mapped {nullptr};

        vk::DescriptorImageInfo descImageInfo { };
        friend class ImageManager;

       public:
        operator bool( ) const { return image.operator bool( ); }

        void destroy( );
        void bind( ) const;

        vk::Device    device;
        vk::Image     image {nullptr};
        vk::Extent3D  extent;
        vk::ImageView view {nullptr};
        vk::Sampler   sampler {nullptr};
        vk::Format    format {vk::Format::eUndefined};
        uint32_t      mipLevels {1};
        uint32_t      layerCount {1};

        AllocationProperties allocatedMemory { };

        void setupDescriptorImageInfo (const vk::ImageLayout& layout);

        const vk::DescriptorImageInfo& getDescImageInfo( ) const;

        template <typename T = void> T* map (vk::DeviceSize size = VK_WHOLE_SIZE)
        {
            mapped = device.mapMemory (allocatedMemory.handle, allocatedMemory.offset, size, vk::MemoryMapFlags( ));
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
    };
}  // namespace GraphicCore

#endif /* IMAGE_HPP */
