#include "EntityRenderer.hpp"

EntityRenderer::EntityRenderer (const GameCore::VulkanDevice& Context, const GameCore::SwapChain& SwapChain,
                                Camera& _camera, const Light& Light)
    : context (Context)
    , swapChain (SwapChain)
    , camera (_camera)
    , light (Light)
{
    // msaaSamples = context.getDevice( ).getMaxUsableSampleCount( );
}

void EntityRenderer::create (const vk::RenderPass& renderPass)
{
    const auto descCount = swapChain.getImageCount( );

    entityDesc.createDescriptorSetLayout( );
    entityDesc.createDescriptorPool (descCount);
    entityDesc.createDescriptorSets (descCount);

    shader.init (renderPass);

    for (auto const& entity : entities)
    {
        for (uint32_t i = 0; i < descCount; ++i)
        {
            entityDesc.addDescriptorWrite (uniform.getUniformBuffers( ) [i].getDescriptorInfo( ),
                                           vk::DescriptorType::eUniformBuffer);

            entityDesc.addDescriptorWrite (
                entity->getTexturedModel( )->getModelTexture( )->getImage( ).getDescImageInfo( ),
                vk::DescriptorType::eCombinedImageSampler);

            entityDesc.addDescriptorWrite (uniform2.getUniformBuffers( ) [i].getDescriptorInfo( ),
                                           vk::DescriptorType::eUniformBuffer);

            entityDesc.updateWriteDescriptors( );
        }
    }
}

void EntityRenderer::destroy (bool isSwapChainCleanUp)
{
    if (isSwapChainCleanUp)
    {
        uniform.destroyBuffer (context);
        uniform2.destroyBuffer (context);
    }

    context.getVkDevice( ).destroyCommandPool (context.getCommandPool( ));
}

void EntityRenderer::createUniformBuffers (const glm::mat4& perspective)
{
    uniform.create (context, swapChain.getImageCount( ), sizeof (UniformBufferObject));
    uniform2.create (context, swapChain.getImageCount( ), sizeof (lightUniformBufferObject));
    for (size_t i = 0; i < swapChain.getImageCount( ); ++i) { updateUniformBuffer (i, perspective); }
}

void EntityRenderer::updateUniformBuffer (const uint32_t& currentImage, const glm::mat4& proj)
{
    for (auto const& entity : entities)
    {
        UniformBufferObject ubo = { };
        ubo.model =
            Math::createTransformationMatrix (entity->getPosition( ), entity->getRotation( ), entity->getScale( ));
        ubo.view = Math::createViewMatrix (camera);
        ubo.proj = proj;

        ubo.lightPosition = light.getPosition( );

        // std::cout << glm::to_string (ubo.model) << std::endl;

        uniform.updateUniformBuffer (currentImage, ubo);

        lightUniformBufferObject lubo = { };
        lubo.lightColor               = light.getColor( );
        lubo.reflectivity             = entity->getTexturedModel( )->getModelTexture( )->getReflectivity( );
        lubo.shineDamper              = entity->getTexturedModel( )->getModelTexture( )->getShineDamper( );

        uniform2.updateUniformBuffer (currentImage, lubo);
    }
}

void EntityRenderer::updateDrawCommandBuffer (const vk::CommandBuffer& cmdBufer, const size_t& i)
{
    for (auto const& entity : entities)
    {
        shader.bind (cmdBufer, i);
        std::vector<vk::DeviceSize> offsets {0};

        cmdBufer.bindVertexBuffers (0, entity->getTexturedModel( )->getModel( )->getVCB( ).getBuffer( ), offsets);
        cmdBufer.bindIndexBuffer (entity->getTexturedModel( )->getModel( )->getICB( ).getBuffer( ), 0,
                                  vk::IndexType::eUint32);

        cmdBufer.drawIndexed (entity->getTexturedModel( )->getModel( )->GetIndexCount( ), 1, 0, 0, 0);
    }
}

void EntityRenderer::pushEntity (const std::vector<Entity const*>& Entities) { entities = Entities; }
