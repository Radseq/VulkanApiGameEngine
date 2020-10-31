#ifndef PIPLINE_DYNAMIC_HPP
#define PIPLINE_DYNAMIC_HPP

#include <Util\util.hpp>
#include <vulkan/vulkan.hpp>

namespace GameCore {
    class PipelineDynamic {
        std::vector<vk::DynamicState>      dynamicStateEnables;
        vk::PipelineDynamicStateCreateInfo dynamicState;

       public:
        PipelineDynamic (bool useDynamic = false);
        ~PipelineDynamic( ) = default;
        void                                update( );
        vk::PipelineDynamicStateCreateInfo& getDynamicState( );
        std::vector<vk::DynamicState>&      getDynamicStateEnables( );
    };
}  // namespace GameCore

#endif  // PIPLINE_DYNAMIC_HPP
