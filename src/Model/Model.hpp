#ifndef MODEL_HPP
#define MODEL_HPP

#include <Buffer\CoreBuffer.hpp>
#include <Device\VulkanDevice.hpp>
#include <vector>
#include <vulkan/vulkan.hpp>

#include "ModelDimension.hpp"
#include "ModelParts.hpp"

class Model {
    const GameCore::VulkanDevice& device;

   public:
    GameCore::CoreBuffer vertexCoreBuffer;
    GameCore::CoreBuffer indexCoreBuffer;
    uint32_t             indexCount {0};
    uint32_t             vertexCount {0};

    const GameCore::CoreBuffer& getVCB( ) const { return vertexCoreBuffer; };
    const GameCore::CoreBuffer& getICB( ) const { return indexCoreBuffer; };

    const uint32_t& GetIndexCount( ) const { return indexCount; };

    ModelDimension dim;

    std::vector<ModelPart> parts;

    explicit Model (const GameCore::VulkanDevice& Device);

    // if cant be in destructor make destroy method
    void destroy( ) {
        vertexCoreBuffer.destroy (device);
        indexCoreBuffer.destroy (device);
    }

    const GameCore::VulkanDevice& GetDevice( ) const;
};

#endif
