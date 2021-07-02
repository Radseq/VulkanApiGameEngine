#ifndef SWAPCHAIN_SUPPORT_DETAILS_HPP
#define SWAPCHAIN_SUPPORT_DETAILS_HPP

#include "pch.h"

namespace GraphicCore
{
    class SwapChainSupportDetails
    {
        const vk::PhysicalDevice& physicalDevice;
        const vk::SurfaceKHR&     surface;

        vk::SurfaceCapabilitiesKHR        surfCaps;        // Get physical device surface properties and formats
        std::vector<vk::PresentModeKHR>   presentModes;    // Get available present modes
        std::vector<vk::SurfaceFormatKHR> surfaceFormats;  // Get list of supported surface formats

       public:
        SwapChainSupportDetails (const vk::PhysicalDevice& PhysicalDevice, const vk::SurfaceKHR& Surface);

        [[nodiscard]] const vk::SurfaceCapabilitiesKHR&        getSurfCaps( ) const;
        [[nodiscard]] const std::vector<vk::PresentModeKHR>&   getPresentModes( ) const;
        [[nodiscard]] const std::vector<vk::SurfaceFormatKHR>& getSurfaceFormats( ) const;
    };
}  // namespace GraphicCore

#endif  // SWAPCHAIN_SUPPORT_DETAILS_HPP
