#include "EntityDescriptor.hpp"

EntityDescriptor::EntityDescriptor (const GameCore::VulkanDevice& Context)
    : context (Context)
{
}

void EntityDescriptor::createDescriptorSets (const uint32_t& descSetsCount)
{
    descriptorSets.create (context.getVkDevice( ), descSetsCount, descSetLayout);
}

void EntityDescriptor::createDescriptorPool (const uint32_t& poolSizeDescSetsCount)
{
    descriptorPool.addDescriptorPoolSize (vk::DescriptorType::eUniformBuffer, poolSizeDescSetsCount);
    descriptorPool.addDescriptorPoolSize (vk::DescriptorType::eCombinedImageSampler, poolSizeDescSetsCount);
    descriptorPool.addDescriptorPoolSize (vk::DescriptorType::eUniformBuffer, poolSizeDescSetsCount);

    descriptorPool.create (context.getVkDevice( ), poolSizeDescSetsCount);
}

void EntityDescriptor::createDescriptorSetLayout( )
{
    descSetLayout.addDescriptorSetLayoutBinding (vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eVertex);
    descSetLayout.addDescriptorSetLayoutBinding (vk::DescriptorType::eCombinedImageSampler,
                                                 vk::ShaderStageFlagBits::eFragment);
    descSetLayout.addDescriptorSetLayoutBinding (vk::DescriptorType::eUniformBuffer,
                                                 vk::ShaderStageFlagBits::eFragment);

    std::vector<vk::DescriptorType> typesUsed {vk::DescriptorType::eUniformBuffer,
                                               vk::DescriptorType::eCombinedImageSampler,
                                               vk::DescriptorType::eUniformBuffer};

    descSetLayout.create (context.getVkDevice( ), typesUsed);
}

void EntityDescriptor::destroy( )
{
    descSetLayout.destroy (context.getVkDevice( ));

    context.getVkDevice( ).destroyCommandPool (context.getCommandPool( ));
}

const GameCore::DescriptorSetLayoutBinding& EntityDescriptor::getDescSetLayout( ) const { return descSetLayout; }
const GameCore::DescriptorSets&             EntityDescriptor::getDescriptorSets( ) const { return descriptorSets; }

void EntityDescriptor::updateWriteDescriptors( )
{
    descriptorSets.update( );
    descriptorSets.clearWriteDescSets( );
}
