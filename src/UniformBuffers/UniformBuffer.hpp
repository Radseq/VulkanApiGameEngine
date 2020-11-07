#ifndef UNIFORM_BUFFES_HPP
#define UNIFORM_BUFFES_HPP

#include <Image\ImageManager.hpp>
#include <vector>

class UniformBuffer
{
    std::vector<GraphicCore::CoreBuffer> uniformBuffers;

   public:
    void create (const GraphicCore::VulkanDevice& Device, const uint32_t& count, const uint32_t& size);

    std::vector<GraphicCore::CoreBuffer>& getUniformBuffers( );
    GraphicCore::CoreBuffer&              getUniformBufferIndex (const uint32_t& index);

    /*template <typename T>
    void copyToMemory (const vk::DeviceMemory& memory, const T& data, const vk::DeviceSize& size,
                       size_t offset = 0) const {
        void* mapped = device.mapMemory (memory, offset, size, vk::MemoryMapFlags( ));
        memcpy (mapped, data, static_cast<size_t> (size));
        device.unmapMemory (memory);
    }*/

    template <typename T> void updateUniformBuffer (const uint32_t& index, const T& obj)
    {
        uniformBuffers [index].map (sizeof (obj));
        uniformBuffers [index].copy (obj);
        uniformBuffers [index].unMap( );
    }

    void destroyBuffer (const GraphicCore::VulkanDevice& Device);
};

#endif  // UNIFORM_BUFFES_HPP
