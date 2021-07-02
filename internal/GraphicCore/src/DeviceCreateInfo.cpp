#include "pch.h"
#include "DeviceCreateInfo.hpp"

namespace GraphicCore
{
    void DeviceCreateInfo::addQueueFamily (uint32_t queueFamilyIndex, vk::ArrayProxy<float> priorities)
    {
        deviceQueues.push_back ({{ }, queueFamilyIndex});
        std::vector<float> prioritiesVector;
        prioritiesVector.resize (priorities.size( ));
        memcpy (prioritiesVector.data( ), priorities.data( ), sizeof (float) * priorities.size( ));
        deviceQueuesPriorities.push_back (prioritiesVector);
    }

    void DeviceCreateInfo::addQueueFamily (uint32_t queueFamilyIndex, size_t count)
    {
        std::vector<float> priorities;
        priorities.resize (count);
        std::fill (priorities.begin( ), priorities.end( ), 0.0F);
        addQueueFamily (queueFamilyIndex, priorities);
    }

    void DeviceCreateInfo::update( )
    {
        assert (deviceQueuesPriorities.size( ) == deviceQueues.size( ));
        auto size = deviceQueues.size( );
        for (auto i = 0; i < size; ++i)
        {
            auto& deviceQueue            = deviceQueues [i];
            auto& deviceQueuePriorities  = deviceQueuesPriorities [i];
            deviceQueue.queueCount       = GraphicCore::Util::toUint32t (deviceQueuePriorities.size( ));
            deviceQueue.pQueuePriorities = deviceQueuePriorities.data( );
        }

        this->queueCreateInfoCount = GraphicCore::Util::toUint32t (deviceQueues.size( ));
        this->pQueueCreateInfos    = deviceQueues.data( );
    }
}  // namespace GraphicCore
