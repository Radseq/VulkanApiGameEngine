#ifndef ENTITY_PIPELINE_HPP
#define ENTITY_PIPELINE_HPP

#include <Descriptor/DescriptorSets.hpp>
#include <Pipeline/Pipeline.hpp>
#include <Pipeline/PipelineLayout.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>
#include <glm\gtx\string_cast.hpp>

#include "../Shader/EntSpecLightShader.hpp"
#include "../Shader/LightShader.hpp"
#include "../ShaderDescInterfaces/IShaderDescSet.hpp"
#include "../ShaderDescInterfaces/IShaderPipeline.hpp"
#include "../ShaderDescriptors/DescriptorPools.hpp"
#include "../ShaderInterfaces/IShader.hpp"
#include "../Util/math.hpp"
#include "../entities/entity.hpp"

class EntityPipeline : public IShaderPipeline
{
    const GraphicCore::VulkanDevice&             m_Context;
    vk::Pipeline                                 graphicsPipeline;
    VulkanGame::Ref<GraphicCore::PipelineLayout> pipelineLayout;

    GraphicCore::VertexLayout vertex_layout {{GraphicCore::VertexLayout::Component::VERTEX_COMPONENT_POSITION,
                                              GraphicCore::VertexLayout::Component::VERTEX_COMPONENT_TEXT_COORD,
                                              GraphicCore::VertexLayout::Component::VERTEX_COMPONENT_NORMAL}};

   public:
    explicit EntityPipeline (const GraphicCore::VulkanDevice& Context);

    void CreateGraphicsPipeline ( std::vector<std::string>&& shaderPatchName,
                                 const vk::Extent2D& swapChainWindowSize, const vk::RenderPass& renderPass) override;
    void CreatePipelineLayout (const IShaderDescSet& descSet) override;
    void Bind (const IShaderDescSet& descSet, const vk::CommandBuffer& cmdBufer,
               const size_t& frameIndex) const override;
    void Destroy( ) override;
};
#endif  // ENTITY_PIPELINE_HPP
