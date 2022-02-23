#ifndef ENTITY_MESH_RENDER_HPP
#define ENTITY_MESH_RENDER_HPP

#include "IShader.hpp"
#include "IShaderDescSet.hpp"
#include "IShaderPipeline.hpp"
#include "entity.hpp"
#include "pch.h"

class EntityMeshRender
{
    std::vector<IShader*> m_Shader;
    const Entity&         m_Entity;

    IShaderDescSet*                  m_DescSet;
    VulkanGame::Ref<IShaderPipeline> m_Pipeline;

   public:
    EntityMeshRender (const Entity& entity, IShaderDescSet* descSet, VulkanGame::Ref<IShaderPipeline> pipeline);

    void Draw (const vk::CommandBuffer& cmdBufer, const size_t& i);

    std::vector<IShader*> GetShaders( );
    const Entity&         GetEntity( );
    void                  Destroy( );
};
#endif  // ENTITY_MESH_RENDER_HPP
