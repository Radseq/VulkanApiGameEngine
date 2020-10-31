#include "DescriptorSets.hpp"

namespace GameCore
{
    void GameCore::DescriptorSets::create (const vk::Device& device, const uint32_t& count,
                                           GameCore::DescriptorSetLayoutBinding& descSetLayout)
    {
        std::vector<vk::DescriptorSetLayout> layouts (count, descSetLayout.getDescriptorSetLayout( ));

        vk::DescriptorSetAllocateInfo allocInfo = { };
        allocInfo.descriptorPool                = descPool.getDescriptorPool( );
        allocInfo.descriptorSetCount            = count;
        allocInfo.pSetLayouts                   = layouts.data( );

        descriptorSets.resize (count);
        descriptorSets = device.allocateDescriptorSets (allocInfo);
    }

    void GameCore::DescriptorSets::update( )
    {
        // assingVkDescSet( );

        device.updateDescriptorSets (GameCore::util::to_uint_32_t (descriptorWrites.size( )), descriptorWrites.data( ),
                                     0, nullptr);

        descriptorWrites.clear( );
        // binding = 0;
    }

#ifdef DEBUG
    void GameCore::DescriptorSets::validateWriteDescSet (const vk::WriteDescriptorSet& writeDescSet)
    {
        for (auto& type : descPool.usedDescTypes( ))
        {
            if (writeDescSet.descriptorType == type.first)
            {
                --type.second;
                if (type.second < 0)
                {
                    throw std::runtime_error (
                        "write desc sets uses too much types that can be allowed by desc pool across all desc sets");
                }
                break;
            }
        }
    }
#endif

    void GameCore::DescriptorSets::clearWriteDescSets( )
    {
        descriptorWrites.clear( );
        // binding = 0;
    }

    const vk::DescriptorSet& GameCore::DescriptorSets::getDescriptorSetByIndex (const size_t& i) const
    {
        return descriptorSets [i];
    }

    void GameCore::DescriptorSets::addDescriptorWrite (const vk::WriteDescriptorSet& writeDescSet)
    {
#ifdef DEBUG
        validateWriteDescSet (writeDescSet);
#endif
        descriptorWrites.push_back (writeDescSet);
    }

    /*
    void GameCore::DescriptorSets::assingVkDescSet( ) {
    uint32_t descSetPoint {0};
    auto     descSetsSize = descriptorSets.size( );

    for (size_t i = 0; i < descriptorWrites.size( ); ++i) {
        descriptorWrites.at (i).dstSet = descriptorSets.at (descSetPoint);
        ++descSetPoint;

        if (descSetPoint == descSetsSize) descSetPoint = 0;
    }

    assert (descSetPoint == 0);
    }*/

    // const std::vector<vk::WriteDescriptorSet>& GameCore::DescriptorSets::getWriteDescSets( ) { return
    // descriptorWrites; }

}  // namespace GameCore