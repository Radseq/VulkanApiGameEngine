#ifndef IMAGE_MANAGER_HPP
#define IMAGE_MANAGER_HPP

#include <ImageContainer.hpp>
#include <utility>

#include "CoreBuffeManager.hpp"
#include "CoreImage.hpp"
#include "CoreImageView.hpp"

namespace GraphicCore
{
    class CoreImage;
    class VulkanDevice;
    class CoreBuffeManager;
    class CoreBuffer;
    class CommandBufferManager;
    class CoreBuffeManager;

    class ImageManager
    {
        const VulkanDevice& device;
        using MipData = std::pair<vk::Extent3D, vk::DeviceSize>;

        void allocateImgMemory (std::shared_ptr<GraphicCore::CoreImage> imageResult,
                                const vk::MemoryPropertyFlags&          properties) const;

       public:
        explicit ImageManager (const VulkanDevice& Device);

        std::shared_ptr<GraphicCore::CoreImage> CreateImage (
            const ImageContainer& imgContainer, const vk::Format& format, const vk::ImageTiling& tiling,
            const vk::ImageUsageFlags& usage, const vk::MemoryPropertyFlags& properties,
            const vk::SampleCountFlagBits& numSamples = vk::SampleCountFlagBits::e1);

        void copyBufferToImage (const vk::Buffer& buffer, std::shared_ptr<GraphicCore::CoreImage> coreImage);

        void generateMipmaps (const GraphicCore::CoreImage& coreImage);
        void transitionImageLayout (const GraphicCore::CoreImage&               coreImage,
                                    const GraphicCore::SingleTimeCommandBuffer& cmdBuff,
                                    const vk::ImageLayout& oldLayout, const vk::ImageLayout& newLayout);
        /*
        std::shared_ptr<GraphicCore::CoreImage> stageToDeviceImage (
            vk::ImageCreateInfo& imageCreateInfo, const vk::MemoryPropertyFlags& memoryPropertyFlags,
            const vk::DeviceSize& size, const void* data, const std::vector<MipData>& mipData = { },
            vk::ImageLayout layout = vk::ImageLayout::eShaderReadOnlyOptimal) const;
        */
        template <typename T>
        std::shared_ptr<GraphicCore::CoreImage> stageToDeviceImage (const vk::ImageCreateInfo&     imageCreateInfo,
                                                                    const vk::MemoryPropertyFlags& memoryPropertyFlags,
                                                                    const std::vector<T>&          data) const
        {
            return stageToDeviceImage (imageCreateInfo, memoryPropertyFlags, data.size( ) * sizeof (T),
                                       (void*) data.data( ));
        }

        template <typename T>
        std::shared_ptr<GraphicCore::CoreImage> stageToDeviceImage (const vk::ImageCreateInfo& imageCreateInfo,
                                                                    const std::vector<T>&      data) const
        {
            return stageToDeviceImage (imageCreateInfo, vk::MemoryPropertyFlagBits::eDeviceLocal,
                                       data.size( ) * sizeof (T), (void*) data.data( ));
        }
    };
}  // namespace GraphicCore

#endif /* IMAGE_MANAGER_HPP */
