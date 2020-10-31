#include "SwapChain.hpp"

namespace GameCore {
    // Free all Vulkan resources used by the swap chain
    void SwapChain::destroy (bool IsRecreated) {
        if (IsRecreated) { device->getVkDevice( ).destroySwapchainKHR (swapChain); }
    }

    const vk::SwapchainKHR&      SwapChain::getVkSwapChain( ) { return swapChain; }
    const vk::Extent2D           SwapChain::getSwapChainExtent( ) const { return Extend; }
    const std::vector<vk::Image> SwapChain::getSwapChainImages( ) const { return images; }
    const uint32_t               SwapChain::getImageCount( ) const { return imageCount; }
    const vk::Format&            SwapChain::getColorFormat( ) const { return colorFormat; }

    void SwapChain::chooseSwapSurfaceFormat( ) {
        const auto formatCount = surfaceFormats.size( );

        // If the surface format list only includes one entry with  vk::Format::eUndefined,
        // there is no preferered format, so we assume  vk::Format::eB8G8R8A8Unorm
        if ((formatCount == 1) && (surfaceFormats [0].format == vk::Format::eUndefined)) {
            colorFormat = vk::Format::eB8G8R8A8Unorm;
        } else {
            // Always select the first available color format
            // If you need a specific format (e.g. SRGB) you'd need to
            // iterate over the list of available surface format and
            // check for it's presence
            colorFormat = surfaceFormats [0].format;
        }
    }

    const uint32_t SwapChain::NumberOfSwapchainImages( ) const {
        uint32_t imagesNumber = surfCaps.minImageCount + 1;
        if ((surfCaps.maxImageCount > 0) && (imagesNumber > surfCaps.maxImageCount)) {
            imagesNumber = surfCaps.maxImageCount;
        }

        return imagesNumber;
    }

    SwapChain::SwapChain (const VulkanDevice* Device)
        : device (Device) { }

    void SwapChain::CreateSwapChain (vk::Extent2D& windowSize, const vk::SurfaceKHR& surface) {
        /*if (windowSize == vk::Extent2D())
        {
                SetExtend();
        }
                // width and height are either both -1, or both not -1.
        else if (surfCaps.currentExtent.width == -1)
        {
                // If the surface size is undefined, the size is set to
                // the size of the images requested.
                Extend = windowSize;
        }
        else
        {
                // If the surface size is defined, the swap chain size must match
                Extend = surfCaps.currentExtent;
                windowSize = surfCaps.currentExtent;
        }*/

        const SwapChainSupportDetails swapChainDetails (device->getDevice( ).GetPhysicalDevice( ), surface);

        surfCaps       = swapChainDetails.getSurfCaps( );
        surfaceFormats = swapChainDetails.getSurfaceFormats( );
        presentModes   = swapChainDetails.getPresentModes( );

        if (windowSize != vk::Extent2D( )) { surfCaps.currentExtent = windowSize; }

        Extend = surfCaps.currentExtent;

        // SetExtend ();

        vk::SwapchainCreateInfoKHR swapChainCI;

        const vk::SwapchainKHR oldSwapchain = swapChain;

        chooseSwapSurfaceFormat( );

        swapChainCI.surface         = surface;
        swapChainCI.minImageCount   = NumberOfSwapchainImages( );
        swapChainCI.imageFormat     = colorFormat;
        swapChainCI.imageColorSpace = surfaceFormats [0].colorSpace;
        swapChainCI.imageExtent     = vk::Extent2D {surfCaps.currentExtent.width, surfCaps.currentExtent.height};
        // swapChainCI.imageUsage = vk::ImageUsageFlagBits::eColorAttachment; //| vk::ImageUsageFlagBits::eTransferDst;
        swapChainCI.imageUsage = GetSupportedImageFlags( );
        // VK_IMAGE_USAGE_TRANSFER_DST_BIT specifies that the image can be used as the destination of a transfer
        // command. VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT specifies that the image can be used to create a VkImageView
        // suitable for use as a color or resolve attachment in a VkFramebuffer.

        swapChainCI.preTransform     = GetSurfaceTransformFlag( );
        swapChainCI.imageArrayLayers = 1;
        auto indices                 = device->getDevice( ).ReturnQueueIndices( );

        if (indices.graphics != indices.present) {
            uint32_t queueFamilyIndices []    = {indices.graphics, indices.present};
            swapChainCI.imageSharingMode      = vk::SharingMode::eConcurrent;
            swapChainCI.queueFamilyIndexCount = 2;
            swapChainCI.pQueueFamilyIndices   = queueFamilyIndices;
        } else {
            swapChainCI.imageSharingMode = vk::SharingMode::eExclusive;
        }

        swapChainCI.queueFamilyIndexCount = 0;
        swapChainCI.pQueueFamilyIndices   = nullptr;
        swapChainCI.presentMode           = GetPresentMode( );

        if (oldSwapchain != vk::SwapchainKHR( )) { swapChainCI.oldSwapchain = oldSwapchain; }

        swapChainCI.clipped        = VK_TRUE;
        swapChainCI.compositeAlpha = findSupportedCompositeAlphaFormat( );

        swapChain = device->getVkDevice( ).createSwapchainKHR (swapChainCI);

        // If an existing sawp chain is re-created, destroy the old swap chain
        // This also cleans up all the presentable images
        if (oldSwapchain) { destroy (oldSwapchain); }

        createVkImage (colorFormat);
    }

    void SwapChain::createVkImage (const vk::Format& colorFormat) {
        // Get the swap chain images
        images     = device->getVkDevice( ).getSwapchainImagesKHR (swapChain);
        imageCount = uint32_t (images.size( ));
    }

    void SwapChain::SetExtend( ) {
        vk::Extent2D swap_chain_extent = {640, 480};

        if (swap_chain_extent.width < surfCaps.minImageExtent.width) {
            swap_chain_extent.width = surfCaps.minImageExtent.width;
        }

        if (swap_chain_extent.height < surfCaps.minImageExtent.height) {
            swap_chain_extent.height = surfCaps.minImageExtent.height;
        }

        if (swap_chain_extent.width > surfCaps.maxImageExtent.width) {
            swap_chain_extent.width = surfCaps.maxImageExtent.width;
        }

        if (swap_chain_extent.height > surfCaps.maxImageExtent.height) {
            swap_chain_extent.height = surfCaps.maxImageExtent.height;
        }

        Extend = swap_chain_extent;
    }

    const vk::ImageUsageFlags SwapChain::GetSupportedImageFlags( ) const {
        vk::ImageUsageFlags flags;

        /*VK_IMAGE_USAGE_TRANSFER_SRC_BIT specifies that the image can be used as the source of a transfer command.

        VK_IMAGE_USAGE_TRANSFER_DST_BIT specifies that the image can be used as the destination of a transfer command.

        VK_IMAGE_USAGE_SAMPLED_BIT specifies that the image can be used to create a VkImageView suitable for occupying a
        VkDescriptorSet slot either of type VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE or
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, and be sampled by a shader.

        VK_IMAGE_USAGE_STORAGE_BIT specifies that the image can be used to create a VkImageView suitable for occupying a
        VkDescriptorSet slot of type VK_DESCRIPTOR_TYPE_STORAGE_IMAGE.

        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT specifies that the image can be used to create a VkImageView suitable for
        use as a color or resolve attachment in a VkFramebuffer.

        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT specifies that the image can be used to create a VkImageView
        suitable for use as a depth / stencil or depth / stencil resolve attachment in a VkFramebuffer.

        VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT specifies that the memory bound to this image will have been allocated
        with the VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT(see Memory Allocation for more detail). This bit can be set for
        any image that can be used to create a VkImageView suitable for use as a color, resolve, depth / stencil, or
        input attachment.

        VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT specifies that the image can be used to create a VkImageView suitable for
        occupying VkDescriptorSet slot of type VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT; be read from a shader as an input
        attachment;and be used as an input attachment in a framebuffer.

        VK_IMAGE_USAGE_SHADING_RATE_IMAGE_BIT_NV specifies that the image can be used to create a VkImageView suitable
        for use as a shading rate image.
        */

        // Is any way to write this better?

        if (surfCaps.supportedUsageFlags & vk::ImageUsageFlagBits::eColorAttachment) {
            flags |= vk::ImageUsageFlagBits::eColorAttachment;
        }

        if (surfCaps.supportedUsageFlags & vk::ImageUsageFlagBits::eTransferSrc) {
            flags |= vk::ImageUsageFlagBits::eTransferSrc;
        }

        if (surfCaps.supportedUsageFlags & vk::ImageUsageFlagBits::eTransferDst) {
            flags |= vk::ImageUsageFlagBits::eTransferDst;
        }
        /*
        if (surfCaps.supportedUsageFlags & vk::ImageUsageFlagBits::eSampled)
        {
                flags |= vk::ImageUsageFlagBits::eSampled;
        }

        if (surfCaps.supportedUsageFlags & vk::ImageUsageFlagBits::eStorage)
        {
                flags |= vk::ImageUsageFlagBits::eStorage;
        }

        if (surfCaps.supportedUsageFlags & vk::ImageUsageFlagBits::eDepthStencilAttachment)
        {
                flags |= vk::ImageUsageFlagBits::eDepthStencilAttachment;
        }

        if (surfCaps.supportedUsageFlags & vk::ImageUsageFlagBits::eTransientAttachment)
        {
                flags |= vk::ImageUsageFlagBits::eTransientAttachment;
        }

        if (surfCaps.supportedUsageFlags & vk::ImageUsageFlagBits::eInputAttachment)
        {
                flags |= vk::ImageUsageFlagBits::eInputAttachment;
        }
        */
        return flags;
    }

    const vk::PresentModeKHR SwapChain::GetPresentMode( ) {
        /*VK_PRESENT_MODE_IMMEDIATE_KHR		: Images submitted by your application are transferred to the screen
        right away, which may result in tearing. VK_PRESENT_MODE_FIFO_KHR			: The swap chain is a
        queue where the display takes an image from the front of the queue when the display is refreshedand the program
        inserts rendered images at the back of the queue. If the queue is full then the program has to wait.This is most
        similar to vertical sync as found in modern games. The moment that the display is refreshed is known as
        "vertical blank".

        VK_PRESENT_MODE_FIFO_RELAXED_KHR	: This mode only differs from the previous one if the application is
        lateand the queue was empty at the last vertical blank. Instead of waiting for the next vertical blank, the
        image is transferred right away when it finally arrives.This may result in visible tearing.

        VK_PRESENT_MODE_MAILBOX_KHR			: This is another variation of the second mode.
                                                                                        Instead of blocking the
        application when the queue is full, the images that are already queued are simply replaced with the newer ones.
                                                                                        This mode can be used to
        implement triple buffering, which allows you to avoid tearing with significantly less latency issues than
        standard vertical sync that uses double buffering.
        */

        for (const auto& present_mode : presentModes) {
            if (present_mode == vk::PresentModeKHR::eMailbox) { return present_mode; }
        }
        for (const auto& present_mode : presentModes) {
            if (present_mode == vk::PresentModeKHR::eImmediate) { return present_mode; }
        }

        return vk::PresentModeKHR::eFifo;
    }

    const vk::SurfaceTransformFlagBitsKHR SwapChain::GetSurfaceTransformFlag( ) const {
        vk::SurfaceTransformFlagBitsKHR preTransform;
        if (surfCaps.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity) {
            preTransform = vk::SurfaceTransformFlagBitsKHR::eIdentity;
        } else {
            preTransform = surfCaps.currentTransform;
        }
        return preTransform;
    }

    const vk::CompositeAlphaFlagBitsKHR SwapChain::findSupportedCompositeAlphaFormat( ) {
        // not all devices support alpha opaque(returned value)

        std::array<vk::CompositeAlphaFlagBitsKHR, 4> compositeAlphaFlags = {
            vk::CompositeAlphaFlagBitsKHR::eOpaque,
            vk::CompositeAlphaFlagBitsKHR::ePreMultiplied,
            vk::CompositeAlphaFlagBitsKHR::ePostMultiplied,
            vk::CompositeAlphaFlagBitsKHR::eInherit,
        };
        for (const auto& compositeAlphaFlag : compositeAlphaFlags) {
            if (surfCaps.supportedCompositeAlpha & compositeAlphaFlag) {
                return compositeAlphaFlag;  // so it return copy(good) or reference(bad)?
                break;
            };
        }
        return vk::CompositeAlphaFlagBitsKHR::eOpaque;
    }
}  // namespace GameCore
