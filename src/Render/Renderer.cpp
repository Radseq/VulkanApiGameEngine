#include "Renderer.hpp"

/// <summary>
/// Initializes a new instance of the <see cref="Renderer"/> class.
/// </summary>
/// <param name="SwapChain">The swap chain.</param>
/// <param name="Context">The context.</param>
/// <param name="_camera">The camera.</param>
Renderer::Renderer (GameCore::SwapChain* SwapChain, GameCore::VulkanDevice* Context, Camera& _camera)
    : swapChain (SwapChain)
    , context (Context)
    , camera (_camera)
{
    initSemaphores( );
}

/// <summary>
/// Draws this instance.
/// </summary>
void Renderer::draw( )
{
    // Get next image in the swap chain (back/front buffer)
    prepareFrame( );

    // terrain setupQueryResultBuffer();

    createNewRenderCommandBuffers( );

    // Execute the compiled command buffer for the current swap chain image
    drawCurrentCommandBuffer( );
    // Push the rendered frame to the surface
    submitFrame( );
}

/// <summary>
/// Prepares the frame.
/// </summary>
void Renderer::prepareFrame( )
{
    frameBufferResized = false;

    const vk::Result fenceRes =
        context->getVkDevice( ).waitForFences (1, &inFlightFences [currentFrame], VK_TRUE, UINT64_MAX);

    if (fenceRes != vk::Result::eSuccess) { throw std::runtime_error ("Error in waitForFences!"); }

    // Acquire the next image from the swap chain
    const auto resultValue = context->getVkDevice( ).acquireNextImageKHR (
        swapChain->getVkSwapChain( ), UINT64_MAX, imageAvailableSemaphores [currentFrame], nullptr);

    if (resultValue.result != vk::Result::eSuccess && resultValue.result != vk::Result::eSuboptimalKHR)
    { throw std::runtime_error ("failed to acquire swap chain image!"); }

    if (resultValue.result == vk::Result::eErrorOutOfDateKHR)
    {
        frameBufferResized = true;
        return;
    }

    currentImage = resultValue.value;
}

/// <summary>
/// Draws the current command buffer.
/// </summary>
void Renderer::drawCurrentCommandBuffer( )
{
    if (frameBufferResized) return;

    vk::SubmitInfo submitInfo = { };
    waitSemaphores            = imageAvailableSemaphores [currentFrame];
    waitStages                = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    signalSemaphores          = renderFinishedSemaphores [currentFrame];

    submitInfo.waitSemaphoreCount   = 1;
    submitInfo.pWaitSemaphores      = &imageAvailableSemaphores [currentFrame];
    submitInfo.pWaitDstStageMask    = &waitStages;
    submitInfo.commandBufferCount   = 1;
    submitInfo.pCommandBuffers      = &commandBuffers [currentImage];
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = &renderFinishedSemaphores [currentFrame];

    context->getVkDevice( ).resetFences (1, &inFlightFences [currentFrame]);

    const vk::Result submitRes = context->getGraphicsQueue( ).submit (1, &submitInfo, inFlightFences [currentFrame]);

    if (submitRes != vk::Result::eSuccess)
    { throw std::runtime_error ("failed to submit draw command buffer! " + vk::to_string (submitRes)); }
}

/// <summary>
/// Submits the frame.
/// </summary>
void Renderer::submitFrame( )
{
    if (frameBufferResized) return;

    vk::PresentInfoKHR presentInfo;
    presentInfo.waitSemaphoreCount = signalSemaphores ? 1 : 0;
    presentInfo.pWaitSemaphores    = &signalSemaphores;
    presentInfo.pImageIndices      = &currentImage;
    presentInfo.swapchainCount     = 1;
    presentInfo.pSwapchains        = &swapChain->getVkSwapChain( );

    // Present the current image to the queue
    const vk::Result res = context->getPresentQueue( ).presentKHR (presentInfo);

    if (res == vk::Result::eErrorOutOfDateKHR || res == vk::Result::eSuboptimalKHR)
    {
        frameBufferResized = true;
        return;
    }

    if (res != vk::Result::eSuccess) { throw std::runtime_error ("failed to present swap chain image!"); }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

/// <summary>
/// Initializes the semaphores.
/// </summary>
void Renderer::initSemaphores( )
{
    // A semaphore used to synchronize image presentation
    // Ensures that the image is displayed before we start submitting new commands to the queu
    imageAvailableSemaphores.resize (MAX_FRAMES_IN_FLIGHT);
    // A semaphore used to synchronize command submission
    // Ensures that the image is not presented until all commands have been sumbitted and executed
    renderFinishedSemaphores.resize (MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize (MAX_FRAMES_IN_FLIGHT);

    vk::FenceCreateInfo fenceCreateInfo;
    fenceCreateInfo.flags = vk::FenceCreateFlagBits::eSignaled;

    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
    {
        imageAvailableSemaphores [i] = context->getVkDevice( ).createSemaphore ({ });
        renderFinishedSemaphores [i] = context->getVkDevice( ).createSemaphore ({ });
        inFlightFences [i]           = context->getVkDevice( ).createFence (fenceCreateInfo);
    }
}

/// <summary>
/// Initializes the specified window size.
/// </summary>
/// <param name="WindowSize">Size of the window.</param>
void Renderer::init (const vk::Extent2D& WindowSize)
{
    windowSize = WindowSize;

    createPerspective( );

    defaultFramebuffer.createRenderPass( );
    createFrameBuffers( );

    allocateCmdBuffers( );

    loadAsserts( );

    entityRenderer.pushEntity (entities);

    entityRenderer.createUniformBuffers (perspective);
    entityRenderer.create (defaultFramebuffer.getVkRenderPass( ));

    createGraphicsPipeline( );

    descPool.addDescriptorPoolSize (vk::DescriptorType::eUniformBuffer, 2);
    descPool.addDescriptorPoolSize (vk::DescriptorType::eCombinedImageSampler, 3);
    constexpr uint32_t maxSize = 2;
    descPool.create (context->getVkDevice( ), maxSize);

    skysphere.loadAssets( );
    terrain.loadAssets( );
    terrain.generateTerrain( );

    skysphere.prepareUniformBuffers (perspective);
    terrain.prepareUniformBuffers (swapChain->getSwapChainExtent( ), perspective);

    createGraphicsPipeline( );

    skysphere.setupDescriptorSets (descPool);
    terrain.setupDescriptorSets (descPool);

    createCommandBuffers( );
}

/// <summary>
/// Creates the graphics pipeline.
/// </summary>
void Renderer::createGraphicsPipeline( )
{
    skysphere.createPipelines (defaultFramebuffer.getVkRenderPass( ));
    terrain.createPipeline (defaultFramebuffer.getVkRenderPass( ));
}

/// <summary>
/// Creates the command buffers.
/// </summary>
void Renderer::createCommandBuffers( )
{
    const vk::CommandBufferBeginInfo cmdBufInfo {vk::CommandBufferUsageFlagBits::eSimultaneousUse};
    const vk::Rect2D                 renderArea = GameCore::vkHelper::rect2D (windowSize);
    const vk::ClearValue clearColor (GameCore::vkHelper::clearColor (glm::vec4 (0.025F, 0.025F, 0.025F, 1.0F)));
    const vk::ClearDepthStencilValue depthStencil = GameCore::vkHelper::getClearValueDepth (1.0F, 0);
    std::vector<vk::ClearValue>      clearColors {clearColor, depthStencil};

    vk::Viewport     viewport = GameCore::vkHelper::viewport (windowSize);
    const vk::Rect2D scissor  = GameCore::vkHelper::rect2D (windowSize);

    vk::RenderPassBeginInfo renderPassInfo;
    renderPassInfo.renderPass      = defaultFramebuffer.getVkRenderPass( );
    renderPassInfo.renderArea      = renderArea;
    renderPassInfo.clearValueCount = GameCore::util::to_uint_32_t (clearColors.size( ));
    renderPassInfo.pClearValues    = clearColors.data( );

    for (size_t i = 0; i < commandBuffers.size( ); ++i)
    {
        renderPassInfo.framebuffer = defaultFramebuffer.getFrameBuffers( ) [i];

        auto& cmdBuffer = commandBuffers [i];

        cmdBuffer.begin (cmdBufInfo);

        updateCommandBufferPreDraw (cmdBuffer);

        cmdBuffer.beginRenderPass (renderPassInfo, vk::SubpassContents::eInline);

        cmdBuffer.setViewport (0, viewport);
        cmdBuffer.setScissor (0, scissor);
        cmdBuffer.setLineWidth (1.0F);

        skysphere.updateDrawCommandBuffer (cmdBuffer);
        terrain.updateDrawCommandBuffer (cmdBuffer);
        entityRenderer.updateDrawCommandBuffer (cmdBuffer, i);

        cmdBuffer.endRenderPass( );
        updateCommandBufferPostDraw (cmdBuffer);
        cmdBuffer.end( );
    }

    // update( );
}

void Renderer::loadAsserts( )
{
    GameCore::Image* textureImage = new GameCore::Image( );
    Model*           model        = new Model (*context);

    TinyObjModelLoader objLoader {*context};
    objLoader.loadFromFile (model, getFilePath( ) + "/../../data/models/dragon/dragon.obj");

    Texture2D        texture;
    stbImgFileLoader stbTextureLoader;

    texture.CreateImage (*textureImage, *context,
                         stbTextureLoader.LoadFile (getFilePath( ) + "/../../data/textures/dragon/white.png"),
                         vk::Format::eR8G8B8A8Srgb);

    textureImage->setupDescriptorImageInfo (vk::ImageLayout::eShaderReadOnlyOptimal);

    assert (textureImage->getDescImageInfo( ) != vk::DescriptorImageInfo( ));

    ModelTexture* mt = new ModelTexture (*textureImage);

    mt->setReflectivity (1.0F);
    mt->setShineDamper (10.0F);

    TexturedModel* tm = new TexturedModel (model, mt);

    glm::vec3 pos {0, -5, -25};
    glm::vec3 rot {0, 0, 0};
    glm::vec3 scale {1, 1, 1};

    Entity* entity = new Entity (tm, pos, rot, scale);

    entities.push_back (entity);
}

/// <summary>
/// Creates the new render command buffers.
/// </summary>
void Renderer::createNewRenderCommandBuffers( ) { }

/// <summary>
/// Creates the frame buffers.
/// </summary>
void Renderer::createFrameBuffers( ) { defaultFramebuffer.CreateFrameBuffer( ); }

/// <summary>
/// Destroys this instance.
/// </summary>
void Renderer::destroy( )
{
    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
    {
        context->getVkDevice( ).destroySemaphore (renderFinishedSemaphores [i]);
        context->getVkDevice( ).destroySemaphore (imageAvailableSemaphores [i]);
        context->getVkDevice( ).destroyFence (inFlightFences [i]);
    }

    skysphere.destroy( );
    terrain.destroy( );
}

/// <summary>
/// Updates this instance.
/// </summary>
void Renderer::update( )
{
    skysphere.updateUniformBuffers (perspective);
    terrain.updateUniformBuffers (swapChain->getSwapChainExtent( ), perspective);
    entityRenderer.updateUniformBuffer (currentImage, perspective);
}

/// <summary>
///  delete resources for recreate
/// </summary>
void Renderer::reCreate( )
{
    defaultFramebuffer.destroy( );

    context->getVkDevice( ).freeCommandBuffers (context->getCommandPool( ), commandBuffers);
}

/// <summary>
/// Updates the command buffer pre draw.
/// </summary>
/// <param name="cmdBuffer">The command buffer.</param>
void Renderer::updateCommandBufferPreDraw (const vk::CommandBuffer& cmdBuffer)
{
    // terrain->updateCommandBufferPreDraw (cmdBuffer);
}

/// <summary>
/// Updates the command buffer post draw.
/// </summary>
/// <param name="cmdBuffer">The command buffer.</param>
void Renderer::updateCommandBufferPostDraw (const vk::CommandBuffer& cmdBuffer) { }

/// <summary>
/// Updates the command buffer.
/// </summary>
/// <param name="cmdBuffer"></param>
void Renderer::updateDrawCommandBuffer (const vk::CommandBuffer& cmdBuffer) { }

void Renderer::queryPoolResult( )
{
    /*
    vkGetQueryPoolResults (GContext.lDevice.device, appRenderData.queryPool, 1, 1, sizeof (uint32_t), &end, 0,
    VK_QUERY_RESULT_WAIT_BIT); vkGetQueryPoolResults (GContext.lDevice.device, appRenderData.queryPool, 0, 1, sizeof
    (uint32_t), &begin, 0, VK_QUERY_RESULT_WAIT_BIT);*/
}

void Renderer::allocateCmdBuffers( )
{
    commandBuffers.resize (defaultFramebuffer.getFrameBuffers( ).size( ));

    commandBuffers =
        context->getVkDevice( ).allocateCommandBuffers ({context->getCommandPool( ), vk::CommandBufferLevel::ePrimary,
                                                         GameCore::util::to_uint_32_t (commandBuffers.size( ))});
}

void Renderer::setUpRenderPass( ) { }

void Renderer::createPerspective( )
{
    constexpr float FOV        = 70;
    constexpr float NEAR_PLANE = 0.1F;
    constexpr float FAR_PLANE  = 512.0F;

    perspective = glm::perspective (glm::radians (FOV),
                                    static_cast<float> (windowSize.width) / static_cast<float> (windowSize.height),
                                    NEAR_PLANE, FAR_PLANE);

    perspective [1][1] *= -1;
}

/// <summary>
/// Determines whether [is frame buffer resized].
/// </summary>
/// <returns></returns>
const bool& Renderer::isFrameBufferResized( ) { return frameBufferResized; }
