#ifndef MODEL_HPP
#define MODEL_HPP

#include <Buffer\CoreBuffer.hpp>
#include <Device\VulkanDevice.hpp>
#include <vector>
#include <vulkan/vulkan.hpp>

#include "ModelDimension.hpp"
#include "ModelParts.hpp"

class Model {
    const GraphicCore::VulkanDevice& device;

   public:
    GraphicCore::CoreBuffer vertexCoreBuffer;
    GraphicCore::CoreBuffer indexCoreBuffer;
    uint32_t             indexCount {0};
    uint32_t             vertexCount {0};

    const GraphicCore::CoreBuffer& getVCB( ) const { return vertexCoreBuffer; };
    const GraphicCore::CoreBuffer& getICB( ) const { return indexCoreBuffer; };

    const uint32_t& GetIndexCount( ) const { return indexCount; };

    ModelDimension dim;

    std::vector<ModelPart> parts;

    explicit Model (const GraphicCore::VulkanDevice& Device);

    // if cant be in destructor make destroy method
    void destroy( ) {
        vertexCoreBuffer.destroy (device);
        indexCoreBuffer.destroy (device);
    }

    const GraphicCore::VulkanDevice& GetDevice( ) const;
};

#endif
