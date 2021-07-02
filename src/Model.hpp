#ifndef MODEL_HPP
#define MODEL_HPP

#include <CoreBuffer.hpp>
#include <VertexLayout.hpp>
#include <VulkanDevice.hpp>

#include "ModelDimension.hpp"
#include "ModelParts.hpp"
#include "pch.h"

class Model
{
    const GraphicCore::VulkanDevice& device;
    const GraphicCore::VertexLayout& m_VertexLayout;

   public:
    GraphicCore::CoreBuffer vertexCoreBuffer;
    GraphicCore::CoreBuffer indexCoreBuffer;
    uint32_t                indexCount {0};
    uint32_t                vertexCount {0};

    const GraphicCore::CoreBuffer& getVCB( ) const { return vertexCoreBuffer; };
    const GraphicCore::CoreBuffer& getICB( ) const { return indexCoreBuffer; };

    const uint32_t& GetIndexCount( ) const { return indexCount; };

    ModelDimension dim;

    std::vector<ModelPart> parts;

    explicit Model (const GraphicCore::VulkanDevice& Device, const GraphicCore::VertexLayout& VertexLayout);

    const GraphicCore::VertexLayout& GetVertexLayout( ) { return m_VertexLayout; }

    void destroy( )
    {
        vertexCoreBuffer.destroy (device);
        indexCoreBuffer.destroy (device);
    }

    const GraphicCore::VulkanDevice& GetDevice( ) const;
};

#endif
