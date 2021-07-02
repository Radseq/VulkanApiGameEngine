#ifndef SWAPCHAIN_HPP
#define SWAPCHAIN_HPP

#include <vulkan/vulkan.hpp>

#include "../Device/VulkanDevice.hpp"
#include "Pipeline/RenderPass.hpp"
#include "SwapChainSupportDetails.hpp"

namespace GraphicCore
{
    /// essentially a queue of images that are waiting to be presented to the screen.
    /// general purpose of the swap chain is to synchronize the presentation of images with the refresh rate of the
    /// screen.

    class PhysicalDevice;
    class RenderPass;

    class SwapChain
    {
        const VulkanDevice* device;

        vk::SurfaceCapabilitiesKHR surfCaps;
        vk::SwapchainKHR           swapChain;
        vk::Format                 colorFormat { };
        vk::Extent2D               Extend;
        uint32_t                   imageCount {0};

        std::vector<vk::SurfaceFormatKHR> surfaceFormats;
        std::vector<vk::PresentModeKHR>   presentModes;
        // std::vector<vk::Framebuffer> swapChainFramebuffers;
        std::vector<vk::Image> images;

        void chooseSwapSurfaceFormat( );
        void SetExtend( );
        void createVkImage (const vk::Format& colorFormat);

        const uint32_t NumberOfSwapchainImages( ) const;

        const vk::ImageUsageFlags             GetSupportedImageFlags( ) const;
        const vk::PresentModeKHR              GetPresentMode( );
        const vk::SurfaceTransformFlagBitsKHR GetSurfaceTransformFlag( ) const;

        const vk::CompositeAlphaFlagBitsKHR findSupportedCompositeAlphaFormat( );

       public:
        SwapChain (const VulkanDevice* Device);

        void CreateSwapChain (vk::Extent2D& windowSize, const vk::SurfaceKHR& surface);
        void destroy (bool IsRecreated = false);

        const vk::SwapchainKHR&      getVkSwapChain( );
        const vk::Extent2D           getSwapChainExtent( ) const;
        const std::vector<vk::Image> getSwapChainImages( ) const;
        const vk::Format&            getColorFormat( ) const;
        const uint32_t               getImageCount( ) const;
    };
}  // namespace GraphicCore

#endif SWAPCHAIN_HPP
