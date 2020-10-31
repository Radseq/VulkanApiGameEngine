#ifndef IVULKAN_INSTANCE_HPP
#define IVULKAN_INSTANCE_HPP

#include <vector>
#include <vulkan/vulkan.hpp>

namespace GameCore {
    class IVulkanInstance {
       public:
        virtual void CreateInstance (const std::string& gameName, const std::vector<std::string>& windowExtensions) = 0;
        virtual const vk::Instance& getVkInstance( )                                                                = 0;

        virtual ~IVulkanInstance( ) = default;
    };
}  // namespace GameCore
#endif IVULKAN_INSTANCE_HPP
