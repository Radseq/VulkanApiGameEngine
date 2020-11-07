#ifndef VULKAN_INSTANCE_HPP
#define VULKAN_INSTANCE_HPP

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <vulkan/vulkan.hpp>

#include "../../../../src/Util/utils.hpp"
#include "../Util/util.hpp"
#include "IVulkanInstance.hpp"

namespace GraphicCore
{
    class IVulkanInstance;

    class VulkanInstance : public IVulkanInstance
    {
        // Vulkan instance, stores all per-application states
        vk::Instance instance;

        // The VK_LAYER_KHRONOS_validation contains all current validation functionality.
        std::vector<std::string> validationLayerAvailable;
        std::vector<const char*> validationLayerUsage;

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

        void availableInstanceLayer( );
        void addValidationLayerToUsage( );

        void assignmentValidationLayers (vk::InstanceCreateInfo* const instanceCreateInfo);

       public:
        VulkanInstance( ) = default;

        void CreateInstance (const std::string_view& gameName, const std::vector<std::string>& windowExtensions) override;
        void destroy( );
        const vk::Instance& getVkInstance( ) override;
    };
}  // namespace GraphicCore
#endif VULKAN_INSTANCE_HPP
