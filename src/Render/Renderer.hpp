#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <Descriptor\DescriptorPool.hpp>

#include "../EntityProcess/EntityRenderer.hpp"
#include "../FileLoader/stbImgFileLoader.hpp"
#include "../Init/DefaultFrameBuffer.hpp"
#include "../Model/TexturedModel.hpp"
#include "../Util/Configuration.hpp"
#include "../World/SkySphere/SkySphere.hpp"
#include "../World/Terrain/Terrain.hpp"
#include "../entities/entity.hpp"
#include "../entities/light.hpp"
#include "Device/VulkanDevice.hpp"
#include "Framebuffer/SwapChain.hpp"
#include "Util/vkHelpers.hpp"

class GraphicCore::SwapChain;
class GraphicCore::VulkanDevice;
class Configuration;

class Renderer
{
    GraphicCore::SwapChain*    swapChain;
    GraphicCore::VulkanDevice* context;

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

    DefaultFrameBuffer defaultFramebuffer {*context, *swapChain};

    Camera& camera;

    Light light {glm::vec3 {0, 0, -20}, glm::vec3 {1, 1, 1}};

    SkySphere skysphere {*context, camera};
    Terrain   terrain {*context, camera};

    GraphicCore::DescriptorPool descPool { };

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

    std::vector<Entity const*> entities;
    void      loadAsserts( );

    EntityRenderer entityRenderer {*context, *swapChain, camera, light};

   public:
    Renderer (GraphicCore::SwapChain* SwapChain, GraphicCore::VulkanDevice* Context, Camera& _camera);

    void draw( );

    void init (const vk::Extent2D& WindowSize);
    void destroy( );
    void update( );
    void reCreate( );

    const bool& isFrameBufferResized( );
};

#endif /* RENDERER_HPP */
