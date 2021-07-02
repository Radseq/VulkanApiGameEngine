#include "EntityRenderer.hpp"

#include <variant>

#include "EntityMeshRender.hpp"

EntityRenderer::EntityRenderer (const GraphicCore::VulkanDevice& Context, const GraphicCore::SwapChain& SwapChain,
                                const Camera& _camera, const Light& Light)
    : context (Context)
    , swapChain (SwapChain)
    , camera (_camera)
    , light (Light)
{
    // msaaSamples = context.getDevice( ).getMaxUsableSampleCount( );
}

void EntityRenderer::destroy (bool isSwapChainCleanUp)
{
    /* if (isSwapChainCleanUp)
     {
         entities.Destroy( );
     }
     */
    entities->Destroy( );
    context.getVkDevice( ).destroyCommandPool (context.getCommandPool( ));
}

void EntityRenderer::createUniformBuffers (const glm::mat4& perspective)
{
    uint32_t ImageCount {swapChain.getImageCount( )};

    for (uint32_t i = 0; i < ImageCount; ++i) { updateUniformBuffer (i, perspective); }
}

void EntityRenderer::updateUniformBuffer (const uint32_t& currentImage, const glm::mat4& proj)
{
    for (auto& ent : entities->GetShaders( ))
    {
        auto entity = entities->GetEntity( );

        ent->SetMat4 ("model", Math::createTransformationMatrix (entity.getPosition( ), entity.getRotation( ),
                                                                 entity.getScale( )));
        ent->SetMat4 ("view", Math::createViewMatrix (camera));
        ent->SetMat4 ("proj", proj);
        ent->SetVec3 ("lightPosition", light.getPosition( ));
        // std::cout << glm::to_string (ubo.model) << std::endl;
        // ent->Update (currentImage);

        ent->SetVec3 ("lightColor", light.getColor( ));
        ent->SetFloat ("shineDamper", entity.getTexturedModel( )->getModelTexture( )->getShineDamper( ));
        ent->SetFloat ("reflectivity", entity.getTexturedModel( )->getModelTexture( )->getReflectivity( ));
        ent->Update (currentImage);
    }
}

void EntityRenderer::updateDrawCommandBuffer (const vk::CommandBuffer& cmdBufer, const size_t& i)
{
    entities->Draw (cmdBufer, i);
}

void EntityRenderer::pushEntity (VulkanGame::Ref<EntityMeshRender> Entities) { entities = Entities; }
