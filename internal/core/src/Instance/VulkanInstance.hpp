#ifndef VULKAN_INSTANCE_HPP
#define VULKAN_INSTANCE_HPP

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <vulkan/vulkan.hpp>

#include "../../../../src/Util/utils.hpp"
#include "IVulkanInstance.hpp"

namespace GameCore {
    class IVulkanInstance;

    class VulkanInstance : public IVulkanInstance {
        // Vulkan instance, stores all per-application states
        vk::Instance instance;

        std::vector<const char*> layers {"VK_LAYER_KHRONOS_validation", "VK_LAYER_LUNARG_standard_validation"};
        const char*              vk_debug_report {"VK_EXT_debug_report"};
        std::vector<const char*> extensions;

        std::vector<vk::ValidationFeatureEnableEXT> featuresEnables {
            vk::ValidationFeatureEnableEXT::eGpuAssisted,
            vk::ValidationFeatureEnableEXT::eGpuAssistedReserveBindingSlot,
            vk::ValidationFeatureEnableEXT::eBestPractices};

        void createDebugReportCallback( );
        void enabledExtensions (const std::vector<std::string>& windowExtensions);

        // std::vector<const char*> enabledLayers();
        // std::vector<const char*> enabledFeatures();
        vk::ValidationFeaturesEXT createValidationFeatures( );
        VkDebugReportCallbackEXT  debugReportCallback { };

       public:
        VulkanInstance( ) = default;

        void CreateInstance (const std::string& gameName, const std::vector<std::string>& windowExtensions) override;
        void destroy( );
        const vk::Instance& getVkInstance( ) override;
    };
}  // namespace GameCore
#endif VULKAN_INSTANCE_HPP
