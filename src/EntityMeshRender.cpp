#include "pch.h"
#include "EntityMeshRender.hpp"

EntityMeshRender::EntityMeshRender (const Entity& entity, IShaderDescSet* descSet,
                                    VulkanGame::Ref<IShaderPipeline> pipeline)
    : m_Shader (descSet->GetShaders( ))
    , m_DescSet (descSet)
    , m_Pipeline (pipeline)
    , m_Entity (entity)
{
}

void EntityMeshRender::Draw (const vk::CommandBuffer& cmdBufer, const size_t& i)
{
    m_Pipeline->Bind (*m_DescSet, cmdBufer, i);
    std::vector<vk::DeviceSize> offsets {0};

    cmdBufer.bindVertexBuffers (0, m_Entity.getTexturedModel( )->getModel( )->getVCB( ).getBuffer( ), offsets);
    cmdBufer.bindIndexBuffer (m_Entity.getTexturedModel( )->getModel( )->getICB( ).getBuffer( ), 0,
                              vk::IndexType::eUint32);

    cmdBufer.drawIndexed (m_Entity.getTexturedModel( )->getModel( )->GetIndexCount( ), 1, 0, 0, 0);
}

void EntityMeshRender::Destroy ()
{
    m_Pipeline->Destroy( );
    m_DescSet->Destroy( );
    delete m_DescSet;
}

std::vector<IShader*> EntityMeshRender::GetShaders( ) { return m_Shader; }

const Entity& EntityMeshRender::GetEntity( ) { return m_Entity; }
