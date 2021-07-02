#ifndef IVULKAN_INSTANCE_HPP
#define IVULKAN_INSTANCE_HPP

#include "pch.h"

namespace GraphicCore
{
    class IVulkanInstance
    {
       public:
        virtual void                CreateInstance (const std::string_view&         gameName,
                                                    const std::vector<std::string>& windowExtensions) = 0;
        virtual const vk::Instance& getVkInstance( )                                                  = 0;

        virtual ~IVulkanInstance( ) = default;
    };
}  // namespace GraphicCore
#endif IVULKAN_INSTANCE_HPP
