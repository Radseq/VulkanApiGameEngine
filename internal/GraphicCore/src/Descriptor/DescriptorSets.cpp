#include "DescriptorSets.hpp"

namespace GraphicCore
{
    void GraphicCore::DescriptorSets::create (const uint32_t& count,
                                              const GraphicCore::DescriptorSetLayoutBinding& descSetLayout)
    {
        std::vector<vk::DescriptorSetLayout> layouts (count, descSetLayout.getDescriptorSetLayout( ));

        vk::DescriptorSetAllocateInfo allocInfo = { };
        allocInfo.descriptorPool                = descPool.getDescriptorPool( );
        allocInfo.descriptorSetCount            = count;
        allocInfo.pSetLayouts                   = layouts.data( );

        descriptorSets.resize (count);
        descriptorSets = device.allocateDescriptorSets (allocInfo);
    }

    void GraphicCore::DescriptorSets::update( )
    {
        device.updateDescriptorSets (GraphicCore::util::to_uint_32_t (descriptorWrites.size( )),
                                     descriptorWrites.data( ), 0, nullptr);

        descriptorWrites.clear( );
    }

#ifdef DEBUG
    void GraphicCore::DescriptorSets::validateWriteDescSet (const vk::WriteDescriptorSet& writeDescSet)
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

   // void GraphicCore::DescriptorSets::clearWriteDescSets( ) { descriptorWrites.clear( ); }

    const vk::DescriptorSet& GraphicCore::DescriptorSets::getDescriptorSetByIndex (const size_t& i) const
    {
        return descriptorSets [i];
    }

    void GraphicCore::DescriptorSets::addDescriptorWrite (const vk::WriteDescriptorSet&& writeDescSet)
    {
#ifdef DEBUG
        validateWriteDescSet (writeDescSet);
#endif
        descriptorWrites.push_back (writeDescSet);
    }

    const uint32_t DescriptorSets::GetDescSetCount( ) const { return static_cast<uint32_t> (descriptorSets.size( )); }

}  // namespace GraphicCore