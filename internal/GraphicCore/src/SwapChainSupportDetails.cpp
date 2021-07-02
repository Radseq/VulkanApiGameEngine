#include "pch.h"
#include "SwapChainSupportDetails.hpp"

namespace GraphicCore
{
    SwapChainSupportDetails::SwapChainSupportDetails (const vk::PhysicalDevice& PhysicalDevice,
                                                      const vk::SurfaceKHR&     Surface)
        : physicalDevice (PhysicalDevice)
        , surface (Surface)
    {
        surfCaps       = physicalDevice.getSurfaceCapabilitiesKHR (surface);
        presentModes   = physicalDevice.getSurfacePresentModesKHR (surface);
        surfaceFormats = physicalDevice.getSurfaceFormatsKHR (surface);
    }

    const vk::SurfaceCapabilitiesKHR&      SwapChainSupportDetails::getSurfCaps( ) const { return surfCaps; }
    const std::vector<vk::PresentModeKHR>& SwapChainSupportDetails::getPresentModes( ) const { return presentModes; }

    const std::vector<vk::SurfaceFormatKHR>& SwapChainSupportDetails::getSurfaceFormats( ) const
    {
        return surfaceFormats;
    }
}  // namespace GraphicCore
