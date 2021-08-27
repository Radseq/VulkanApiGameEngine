#include "pch.h"
#include "Renderer.hpp"

/// <summary>
/// Initializes a new instance of the <see cref="Renderer"/> class.
/// </summary>
/// <param name="SwapChain">The swap chain.</param>
/// <param name="Context">The context.</param>
/// <param name="_camera">The camera.</param>
Renderer::Renderer (std::unique_ptr<GraphicCore::SwapChain>& SwapChain, GraphicCore::VulkanDevice* Context,
                    Camera& _camera)
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
    {
        throw std::runtime_error ("failed to acquire swap chain image!");
    }

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

    vk::Result operationResult;

    operationResult = context->getVkDevice( ).resetFences (1, &inFlightFences [currentFrame]);

    if (operationResult != vk::Result::eSuccess)
    {
        throw std::runtime_error ("failed to reset fence! " + vk::to_string (operationResult));
    }

    operationResult = context->getGraphicsQueue( ).submit (1, &submitInfo, inFlightFences [currentFrame]);

    if (operationResult != vk::Result::eSuccess)
    {
        throw std::runtime_error ("failed to submit draw command buffer! " + vk::to_string (operationResult));
    }
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

    createFrameBuffers( );

    allocateCmdBuffers( );

    loadAsserts( );

    entityRenderer.pushEntity (EntRendObj);

    entityRenderer.createUniformBuffers (perspective, swapChain->getImageCount( ));

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

    skysphere.setupDescriptorSets (descPool);
    terrain.setupDescriptorSets (descPool);

    createCommandBuffers( );
}

/// <summary>
/// Creates the graphics pipeline.
/// </summary>
void Renderer::createGraphicsPipeline( )
{
    skysphere.createPipelines (renderPass.GetVkRenderPass( ));
    terrain.createPipeline (renderPass.GetVkRenderPass( ));
}

/// <summary>
/// Creates the command buffers.
/// </summary>
void Renderer::createCommandBuffers( )
{
    const vk::CommandBufferBeginInfo cmdBufInfo {vk::CommandBufferUsageFlagBits::eSimultaneousUse};
    const vk::Rect2D                 renderArea = GraphicCore::vkHelper::rect2D (windowSize);
    const vk::ClearValue clearColor (VulkanGame::VkHelpers::clearColor (glm::vec4 (0.025F, 0.025F, 0.025F, 1.0F)));
    const vk::ClearDepthStencilValue depthStencil = GraphicCore::vkHelper::getClearValueDepth (1.0F, 0);
    std::vector<vk::ClearValue>      clearColors {clearColor, depthStencil};

    vk::Viewport     viewport = GraphicCore::vkHelper::viewport (windowSize);
    const vk::Rect2D scissor  = GraphicCore::vkHelper::rect2D (windowSize);

    vk::RenderPassBeginInfo renderPassInfo;
    renderPassInfo.renderPass      = renderPass.GetVkRenderPass( );
    renderPassInfo.renderArea      = renderArea;
    renderPassInfo.clearValueCount = GraphicCore::Util::toUint32t (clearColors.size( ));
    renderPassInfo.pClearValues    = clearColors.data( );

    for (size_t i = 0; i < commandBuffers.size( ); ++i)
    {
        renderPassInfo.framebuffer = swapChainFramebuffers [i];

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
    GraphicCore::VertexLayout vertexLayout {{GraphicCore::VertexLayout::Component::VERTEX_COMPONENT_POSITION,
                                             GraphicCore::VertexLayout::Component::VERTEX_COMPONENT_TEXT_COORD,
                                             GraphicCore::VertexLayout::Component::VERTEX_COMPONENT_NORMAL}};

    // CoreImage (const vk::Device& Device, const vk::Extent3D& Extent, const vk::Format& Format,
    //   const vk::ImageUsageFlags& UsageFlags, const vk::MemoryPropertyFlags& MemoryProperties);

    VulkanGame::Ref<GraphicCore::CoreImage> textureImage;

    Model* model = new Model (*context, vertexLayout);

    TinyObjModelLoader objLoader {*context};
    objLoader.loadFromFile (model, ResourcePatch::GetInstance( )->GetPatch ("DataPatch") + "/models/dragon/dragon.obj");

    VulkanGame::Ref<Texture2D> texture;
    stbImgFileLoader           stbTextureLoader;

    textureImage =
        texture->CreateImage (*context,
                              stbTextureLoader.LoadFile (ResourcePatch::GetInstance( )->GetPatch ("DataPatch") +
                                                         "/textures/dragon/white.png"),
                              vk::Format::eR8G8B8A8Srgb);

    vk::Bool32 samplerAnisotropy = context->getDevice( ).GetPhysicalDeviceFeatures( ).samplerAnisotropy;

    vk::SamplerCreateInfo samplerInfo = { };
    samplerInfo.magFilter             = vk::Filter::eLinear;
    samplerInfo.minFilter             = vk::Filter::eLinear;
    samplerInfo.addressModeU          = vk::SamplerAddressMode::eRepeat;
    samplerInfo.addressModeV          = vk::SamplerAddressMode::eRepeat;
    samplerInfo.addressModeW          = vk::SamplerAddressMode::eRepeat;
    samplerInfo.anisotropyEnable      = samplerAnisotropy;
    samplerInfo.maxAnisotropy =
        samplerAnisotropy ? context->getDevice( ).GetDeviceProperties( ).limits.maxSamplerAnisotropy : 1.0F;
    samplerInfo.borderColor             = vk::BorderColor::eFloatOpaqueWhite;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable           = VK_FALSE;
    samplerInfo.compareOp               = vk::CompareOp::eNever;
    samplerInfo.mipmapMode              = vk::SamplerMipmapMode::eLinear;
    samplerInfo.minLod                  = 0;
    samplerInfo.maxLod                  = static_cast<float> (textureImage->GetMipLevels( ));
    samplerInfo.mipLodBias              = 0;

    textureImageSampler.Create (samplerInfo);

    textureDescImageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
    for (auto& view : textureImage->GetViews( )) { textureDescImageInfo.imageView = view->GetImageView( ); }
    textureDescImageInfo.sampler = textureImageSampler.GetVkSampler( );

    VulkanGame::Ref<ModelTexture> mt = VulkanGame::CreateRef<ModelTexture> (textureImage);

    mt->setReflectivity (1.0F);
    mt->setShineDamper (10.0F);

    VulkanGame::Ref<TexturedModel> tm = VulkanGame::CreateRef<TexturedModel> (model, mt);

    glm::vec3 pos {0, -5, -25};
    glm::vec3 rot {0, 0, 0};
    glm::vec3 scale {1, 1, 1};

    Entity* entity = new Entity (tm, pos, rot, scale);

    VulkanGame::Ref<IShaderDescSet> entShader = VulkanGame::CreateRef<EntityDesc> (*context, textureDescImageInfo);

    entShader->CreateDescriptorSetLayout( );
    entShader->CreateDescriptorSets (3);

    VulkanGame::Ref<IShaderPipeline> entPipeline = VulkanGame::CreateRef<EntityPipeline> (*context);

    entPipeline->CreatePipelineLayout (*entShader.get( ));

    std::vector<std::string> shaderPatchName {
        ResourcePatch::GetInstance( )->GetPatch ("DataPatch") + "/shaders/static_shader/specular_lighting/vert.spv",
        ResourcePatch::GetInstance( )->GetPatch ("DataPatch") + "/shaders/static_shader/specular_lighting/frag.spv"};

    entPipeline->CreateGraphicsPipeline (std::move (shaderPatchName), swapChain->getSwapChainExtent( ),
                                         renderPass.GetVkRenderPass( ), vertexLayout);

    EntRendObj = VulkanGame::CreateRef<EntityMeshRender> (*entity, entShader, entPipeline);
}

/// <summary>
/// Creates the new render command buffers.
/// </summary>
void Renderer::createNewRenderCommandBuffers( ) { }

/// <summary>
/// Creates the frame buffers.
/// </summary>
void Renderer::createFrameBuffers( )
{
    vk::Extent3D swapChainExtent = vk::Extent3D (swapChain->getSwapChainExtent( ), 1);

    for (size_t i = 0; i < swapChain->getSwapChainImages( ).size( ); ++i)
    {
        VulkanGame::Ref<GraphicCore::CoreImage> swapchainImage = VulkanGame::CreateRef<GraphicCore::CoreImage> (
            *context, swapChain->getSwapChainImages( ) [i], swapChainExtent, swapChain->getColorFormat( ),
            swapChain->GetSupportedImageFlags( ));

        auto renderTarget = fbaFunc (std::move (swapchainImage));

        VulkanGame::PassToVec (Framebuffers, context->getVkDevice( ), renderTarget, renderPass);

        swapChainFramebuffers.push_back (Framebuffers [i].GetVkFrameBuffer( ));
    }
}

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

    entityRenderer.destroy( );

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
    Framebuffers.clear( );
    swapChainFramebuffers.clear( );

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
    commandBuffers.resize (Framebuffers.size( ));

    commandBuffers =
        context->getVkDevice( ).allocateCommandBuffers ({context->getCommandPool( ), vk::CommandBufferLevel::ePrimary,
                                                         GraphicCore::Util::toUint32t (commandBuffers.size( ))});
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
