#include "VulkanInstance.hpp"

namespace GameCore {
#ifdef _DEBUG

    static VKAPI_ATTR VkBool32 debugReportCallbackFunction (VkDebugReportFlagsEXT      flags,
                                                            VkDebugReportObjectTypeEXT objType, uint64_t srcObject,
                                                            size_t location, int32_t msgCode, const char* pLayerPrefix,
                                                            const char* pMsg, void* pUserData) {
        std::string message;
        {
            std::stringstream buf;
            if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
                buf << "ERROR: ";
            } else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
                buf << "WARNING: ";
            } else if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
                buf << "PERF: ";
            } else {
                return false;
            }
            buf << "[" << pLayerPrefix << "] Code " << msgCode << " : " << pMsg;
            message = buf.str( );
        }

        std::cout << message << std::endl;  // console log
        LogMsg (message);                   // logger
        return false;
    }

    void VulkanInstance::createDebugReportCallback( ) {
        // NOTE: instance.createDebugReportCallbackEXT() would be much nicer but is not currently implemented by the API
        // (only declared)
        const PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT =
            reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT> (
                instance.getProcAddr ("vkCreateDebugReportCallbackEXT"));
        if (vkCreateDebugReportCallbackEXT) {
            vk::DebugReportCallbackCreateInfoEXT debugReportCallbackCreateInfo (
                vk::DebugReportFlagBitsEXT::eInformation | vk::DebugReportFlagBitsEXT::eWarning |
                    vk::DebugReportFlagBitsEXT::ePerformanceWarning | vk::DebugReportFlagBitsEXT::eError |
                    vk::DebugReportFlagBitsEXT::eDebug,
                debugReportCallbackFunction);
            auto temp = VkDebugReportCallbackCreateInfoEXT (debugReportCallbackCreateInfo);
            if (vkCreateDebugReportCallbackEXT (instance, &temp, nullptr, &debugReportCallback) != VK_SUCCESS) {
                throw std::runtime_error ("Failed to create debug report callback.");
            }
        }
    }
    /*
    std::vector<const char*> VulkanInstance::enabledLayers ()
    {
            //auto layers = vk::enumerateInstanceLayerProperties();

            //for (const auto& layer : layers) {
            //	enabledLayers.push_back(layer.layerName);
            //}
            //VK_LAYER_LUNARG_assistant_layer
            return std::vector<const char*>{
                    "VK_LAYER_KHRONOS_validation", "VK_LAYER_LUNARG_standard_validation"
            };
    }

    std::vector<const char*> VulkanInstance::enabledFeatures ()
    {
            return std::vector<const char*>{"VK_LAYER_LUNARG_assistant_layer"};
    }*/

    vk::ValidationFeaturesEXT VulkanInstance::createValidationFeatures( ) {
        vk::ValidationFeaturesEXT features;  // slow down ~10%
        features.setEnabledValidationFeatureCount (uint8_t (featuresEnables.size( )));
        features.setPEnabledValidationFeatures (featuresEnables.data( ));
        return features;
    }
#endif

    void VulkanInstance::enabledExtensions (const std::vector<std::string>& windowExtensions) {
        /*auto instanceExtensions = vk::enumerateInstanceExtensionProperties();

        for (const auto& extension : instanceExtensions) {
                enabledExtensions.push_back(extension.extensionName);
        }*/

        for (const auto& extension : windowExtensions) { extensions.push_back (extension.c_str( )); }

        extensions.push_back (vk_debug_report);
    }

    void VulkanInstance::CreateInstance (const std::string&              gameName,
                                         const std::vector<std::string>& windowExtensions) {
        vk::ApplicationInfo    appInfo;
        vk::InstanceCreateInfo instanceCreateInfo;

        // Enable surface extensions depending on os
        enabledExtensions (windowExtensions);

        appInfo.pApplicationName = gameName.c_str( );
        appInfo.pEngineName      = gameName.c_str( );
        // appInfo.pEngineName = "Amazing Engine";
        appInfo.apiVersion = VK_API_VERSION_1_1;  // VK_MAKE_VERSION (1, 2, 0);
        // appInfo.applicationVersion = VK_MAKE_VERSION (1, 2, 0);
        appInfo.engineVersion = VK_MAKE_VERSION (1, 0, 0);

        instanceCreateInfo.pApplicationInfo = &appInfo;

        if (!extensions.empty( )) {
            instanceCreateInfo.enabledExtensionCount   = uint32_t (extensions.size( ));
            instanceCreateInfo.ppEnabledExtensionNames = extensions.data( );
        }

#ifdef _DEBUG
        instanceCreateInfo.enabledLayerCount   = uint32_t (layers.size( ));
        instanceCreateInfo.ppEnabledLayerNames = layers.data( );
        vk::ValidationFeaturesEXT features     = createValidationFeatures( );  // slow down ~10%
        instanceCreateInfo.pNext               = &features;
#endif

        instance = createInstance (instanceCreateInfo);

#ifdef _DEBUG
        // update assimp and this stopped working, wtf is with this world
        /*if (!(std::find (extensions.begin (), extensions.end (), "VK_EXT_debug_report") != extensions.end ())) {
                throw std::runtime_error ("Failed to create DebugReportCallback, first enable VK_EXT_debug_report in
        ppEnabledExtensionNames");
        }

        if (!(std::find (layers.begin (), layers.end (), "VK_LAYER_LUNARG_standard_validation") != layers.end ())) {
                throw std::runtime_error ("Failed to create DebugReportCallback, first enable
        VK_LAYER_LUNARG_standard_validation in ppEnabledLayerNames");
        }*/
        createDebugReportCallback( );
#endif
    }

    const vk::Instance& VulkanInstance::getVkInstance( ) { return instance; }

    void VulkanInstance::destroy( ) {
#ifdef _DEBUG
        const PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT =
            reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT> (
                instance.getProcAddr ("vkDestroyDebugReportCallbackEXT"));
        vkDestroyDebugReportCallbackEXT (instance, debugReportCallback, nullptr);
#endif

        instance.destroy( );
    }
}  // namespace GameCore
