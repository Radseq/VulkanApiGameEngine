#ifndef ENTITY_MESH_RENDER_HPP
#define ENTITY_MESH_RENDER_HPP

#include "pch.h"

#include "IShaderDescSet.hpp"
#include "IShaderPipeline.hpp"
#include "IShader.hpp"
#include "entity.hpp"

class EntityMeshRender
{
    std::vector<VulkanGame::Ref<IShader>> m_Shader;
    const Entity&                         m_Entity;

    VulkanGame::Ref<IShaderDescSet>  m_DescSet;
    VulkanGame::Ref<IShaderPipeline> m_Pipeline;

   public:
    EntityMeshRender (const Entity& entity, VulkanGame::Ref<IShaderDescSet> descSet,
                      VulkanGame::Ref<IShaderPipeline> pipeline);

    void Draw (const vk::CommandBuffer& cmdBufer, const size_t& i);

    std::vector<VulkanGame::Ref<IShader>> GetShaders( );
    const Entity&                         GetEntity( );
    void                                  Destroy( );
};
#endif  // ENTITY_MESH_RENDER_HPP
