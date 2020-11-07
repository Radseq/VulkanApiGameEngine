#include "UniformBuffer.hpp"

void UniformBuffer::create (const GraphicCore::VulkanDevice& Device, const uint32_t& count, const uint32_t& size)
{
    vk::DeviceSize bufferSize = size;

    GraphicCore::CoreBufferManager bufferManager {Device};

    uniformBuffers.resize (count);

    for (size_t i = 0; i < count; i++)
    {
        bufferManager.createSizedUniformBuffer (uniformBuffers [i], bufferSize);
        uniformBuffers [i].setupDescriptor (size);

        assert (uniformBuffers [i].getDescriptorInfo( ) != vk::DescriptorBufferInfo( ));
    }
}

std::vector<GraphicCore::CoreBuffer>& UniformBuffer::getUniformBuffers( ) { return uniformBuffers; }
GraphicCore::CoreBuffer& UniformBuffer::getUniformBufferIndex (const uint32_t& index) { return uniformBuffers [index]; }

void UniformBuffer::destroyBuffer (const GraphicCore::VulkanDevice& Device)
{
    for (auto& ub : uniformBuffers) { ub.destroy (Device); }
}
