#ifndef ENTITY_MESH_RENDER_HPP
#define ENTITY_MESH_RENDER_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>
#include <glm\gtx\string_cast.hpp>

#include "../ShaderDescInterfaces/IShaderDescSet.hpp"
#include "../ShaderDescInterfaces/IShaderPipeline.hpp"
#include "../ShaderInterfaces/IShader.hpp"
#include "../Util/math.hpp"
#include "../entities/entity.hpp"

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
