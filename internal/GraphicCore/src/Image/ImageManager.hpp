#ifndef IMAGE_MANAGER_HPP
#define IMAGE_MANAGER_HPP

#include <Image\ImageContainer.hpp>
#include <utility>
#include <vulkan/vulkan.hpp>

#include "../Device/VulkanDevice.hpp"
#include "../Util/vkHelpers.hpp"
#include "Buffer/CoreBuffeManager.hpp"
#include "Image.hpp"

namespace GraphicCore
{
    class Image;
    class VulkanDevice;
    class CoreBuffeManager;
    class CoreBuffer;
    class CommandBufferManager;
    class CoreBuffeManager;

    class ImageManager
    {
        const VulkanDevice& device;
        using MipData = std::pair<vk::Extent3D, vk::DeviceSize>;

        void allocateImgMemory (GraphicCore::Image& imageResult, const vk::MemoryPropertyFlags& properties) const;

       public:
        explicit ImageManager (const VulkanDevice& Device);

        void CreateImage (GraphicCore::Image& result, const ImageContainer& imgContainer, const vk::Format& format,
                          const vk::ImageTiling& tiling, const vk::ImageUsageFlags& usage,
                          const vk::MemoryPropertyFlags& properties,
                          const vk::SampleCountFlagBits& numSamples = vk::SampleCountFlagBits::e1);

        [[nodiscard]] void CreateImage (
            GraphicCore::Image& result, const vk::ImageCreateInfo& imageCreateInfo,
            const vk::MemoryPropertyFlags& memoryPropertyFlags = vk::MemoryPropertyFlagBits ::eDeviceLocal) const;

        void createSampler (GraphicCore::Image& coreImage);
        void copyBufferToImage (const vk::Buffer& buffer, const GraphicCore::Image& coreImage);
        void createImageView (GraphicCore::Image& coreImage, const vk::ImageAspectFlags& aspectFlags,
                              const vk::ImageViewType& imgViewType);
        void generateMipmaps (const GraphicCore::Image& coreImage);
        void transitionImageLayout (const GraphicCore::Image& coreImage, const GraphicCore::SingleTimeCommandBuffer& cmdBuff,
                                    const vk::ImageLayout& oldLayout,
                                    const vk::ImageLayout& newLayout);

        void stageToDeviceImage (GraphicCore::Image& result, vk::ImageCreateInfo& imageCreateInfo,
                                 const vk::MemoryPropertyFlags& memoryPropertyFlags, const vk::DeviceSize& size,
                                 const void* data, const std::vector<MipData>& mipData = { },
                                 vk::ImageLayout layout = vk::ImageLayout::eShaderReadOnlyOptimal) const;

        template <typename T>
        void stageToDeviceImage (GraphicCore::Image& result, const vk::ImageCreateInfo& imageCreateInfo,
                                 const vk::MemoryPropertyFlags& memoryPropertyFlags, const std::vector<T>& data) const
        {
            return stageToDeviceImage (imageCreateInfo, memoryPropertyFlags, data.size( ) * sizeof (T),
                                       (void*) data.data( ));
        }

        template <typename T>
        void stageToDeviceImage (GraphicCore::Image& result, const vk::ImageCreateInfo& imageCreateInfo,
                                 const std::vector<T>& data) const
        {
            return stageToDeviceImage (imageCreateInfo, vk::MemoryPropertyFlagBits::eDeviceLocal,
                                       data.size( ) * sizeof (T), (void*) data.data( ));
        }
    };
}  // namespace GraphicCore

#endif /* IMAGE_MANAGER_HPP */
