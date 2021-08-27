#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <DescriptorPool.hpp>
#include <Framebuffer.hpp>

#include "Configuration.hpp"
#include "EntityDesc.hpp"
#include "EntityMeshRender.hpp"
#include "EntityPipeline.hpp"
#include "EntityRenderer.hpp"
#include "SkySphere.hpp"
#include "SwapChain.hpp"
#include "Terrain.hpp"
#include "TexturedModel.hpp"
#include "VkUtils.hpp"
#include "VulkanDevice.hpp"
#include "entity.hpp"
#include "light.hpp"
#include "stbImgFileLoader.hpp"
#include "vkHelpers.hpp"

class GraphicCore::SwapChain;
class GraphicCore::VulkanDevice;
class Configuration;

class Renderer
{
    std::unique_ptr<GraphicCore::SwapChain>& swapChain;
    GraphicCore::VulkanDevice*               context;

    glm::mat4 perspective;

    bool frameBufferResized = false;

    const int MAX_FRAMES_IN_FLIGHT = Configuration::getInstance( )->GetConfigIntOption ("MaxFramesInFlight");
    uint32_t  currentImage {0};

    // Active frame buffer index
    size_t currentFrame = 0;

    vk::Extent2D windowSize {0, 0};

    // Synchronization semaphores
    std::vector<vk::Semaphore> imageAvailableSemaphores;
    std::vector<vk::Semaphore> renderFinishedSemaphores;
    std::vector<vk::Fence>     inFlightFences;

    std::vector<vk::CommandBuffer> commandBuffers;

    vk::Semaphore          waitSemaphores;
    vk::Semaphore          signalSemaphores;
    vk::PipelineStageFlags waitStages;

    // DefaultFrameBuffer defaultFramebuffer {*context, *swapChain};

    std::vector<vk::Framebuffer> swapChainFramebuffers;

    std::vector<GraphicCore::vkBasicModels::AttachmentModel> m_AttachemntsVec {
        {swapChain->getColorFormat( ), vk::SampleCountFlagBits::e1,
         vk::ImageUsageFlagBits::eTransientAttachment | vk::ImageUsageFlagBits::eColorAttachment},
        {context->depthFormat, vk::SampleCountFlagBits::e1,
         vk::ImageUsageFlagBits::eDepthStencilAttachment | vk::ImageUsageFlagBits::eTransferSrc}};

    std::vector<GraphicCore::vkBasicModels::LoadStoreInfo> storeInfoVec {{ }, {}};
    GraphicCore::vkBasicModels::SubpassInfo                subPassInfo {{ }, {0}};
    std::vector<GraphicCore::vkBasicModels::SubpassInfo>   subpassInfoVec {subPassInfo};
    GraphicCore::RenderPass renderPass {context->getVkDevice( ), m_AttachemntsVec, storeInfoVec, subpassInfoVec};

    std::vector<GraphicCore::Framebuffer> Framebuffers;

    GraphicCore::FrameBufferAttachment::CreateFunc fbaFunc = GraphicCore::FrameBufferAttachment::DEFAULT_CREATE_FUNC;

    Camera& camera;

    Light light {glm::vec3 {0, 0, -20}, glm::vec3 {1, 1, 1}};

    SkySphere skysphere {*context, camera};
    Terrain   terrain {*context, camera};

    GraphicCore::DescriptorPool descPool { };

    VulkanGame::Ref<EntityMeshRender> EntRendObj;

    void prepareFrame( );
    void drawCurrentCommandBuffer( );
    void submitFrame( );
    void initSemaphores( );
    void createGraphicsPipeline( );
    void createCommandBuffers( );
    void createNewRenderCommandBuffers( );
    void createFrameBuffers( );
    void updateCommandBufferPreDraw (const vk::CommandBuffer& cmdBuffer);
    void updateCommandBufferPostDraw (const vk::CommandBuffer& cmdBuffer);
    void updateDrawCommandBuffer (const vk::CommandBuffer& cmdBuffer);

    void queryPoolResult( );

    void allocateCmdBuffers( );

    void setUpRenderPass( );

    void createPerspective( );

    void loadAsserts( );

    EntityRenderer entityRenderer {*context, camera, light};

    vk::DescriptorImageInfo textureDescImageInfo { };

    GraphicCore::ImageSampler textureImageSampler {context->getVkDevice( )};

   public:
    Renderer (std::unique_ptr<GraphicCore::SwapChain>& SwapChain, GraphicCore::VulkanDevice* Context, Camera& _camera);

    void draw( );

    void init (const vk::Extent2D& WindowSize);
    void destroy( );
    void update( );
    void reCreate( );

    const bool& isFrameBufferResized( );
};

#endif /* RENDERER_HPP */
