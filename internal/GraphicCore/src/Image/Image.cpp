#include "Image.hpp"

namespace GraphicCore {
    void Image::destroy( ) {
        if (device != vk::Device( )) {
            if (sampler) {
                device.destroySampler (sampler);
                sampler = vk::Sampler( );
            }
            if (view) {
                device.destroyImageView (view);
                view = vk::ImageView( );
            }
            if (image) {
                device.destroyImage (image);
                image = vk::Image( );
            }

            // logicDevice.getAllocator ()->free (allocatedMemory); // todo:(
        }
    }

    void Image::bind( ) const { device.bindImageMemory (image, allocatedMemory.handle, allocatedMemory.offset); }

    void Image::copy (const size_t& size, const void* data, const vk::DeviceSize& offset) const {
        memcpy (static_cast<uint8_t*> (mapped) + offset, data, size);
    };

    void Image::setupDescriptorImageInfo (const vk::ImageLayout& layout) {
        assert (view != vk::ImageView( ));
        assert (sampler != vk::Sampler( ));
        descImageInfo.imageLayout = layout;
        descImageInfo.imageView   = view;

        descImageInfo.sampler = sampler;
    }

    void Image::unMap( ) { device.unmapMemory (allocatedMemory.handle); }

    const vk::DescriptorImageInfo& Image::getDescImageInfo( ) const { return descImageInfo; }
}  // namespace GraphicCore
