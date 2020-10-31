#include "UniformBuffer.hpp"

void UniformBuffer::create (const GameCore::VulkanDevice& Device, const uint32_t& count, const uint32_t& size)
{
    vk::DeviceSize bufferSize = size;

    GameCore::CoreBufferManager bufferManager {Device};

    uniformBuffers.resize (count);

    for (size_t i = 0; i < count; i++)
    {
        bufferManager.createSizedUniformBuffer (uniformBuffers [i], bufferSize);
        uniformBuffers [i].setupDescriptor (size);

        assert (uniformBuffers [i].getDescriptorInfo( ) != vk::DescriptorBufferInfo( ));
    }
}

std::vector<GameCore::CoreBuffer>& UniformBuffer::getUniformBuffers( ) { return uniformBuffers; }
GameCore::CoreBuffer& UniformBuffer::getUniformBufferIndex (const uint32_t& index) { return uniformBuffers [index]; }

void UniformBuffer::destroyBuffer (const GameCore::VulkanDevice& Device)
{
    for (auto& ub : uniformBuffers) { ub.destroy (Device); }
}
