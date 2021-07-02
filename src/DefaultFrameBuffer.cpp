#include "pch.h"
#include "DefaultFrameBuffer.hpp"

void DefaultFrameBuffer::ImageViewForSwapChainImg( )
{
    vk::ImageViewCreateInfo colorSwapChainImageView;
    colorSwapChainImageView.format     = swapChain.getColorFormat( );
    colorSwapChainImageView.components = {vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB,
                                          vk::ComponentSwizzle::eA};
    colorSwapChainImageView.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
    colorSwapChainImageView.subresourceRange.levelCount = 1;
    colorSwapChainImageView.subresourceRange.layerCount = 1;
    colorSwapChainImageView.viewType                    = vk::ImageViewType::e2D;

    for (size_t i = 0; i < swapChain.getSwapChainImages( ).size( ); i++)
    {
        colorSwapChainImageView.image = swapChain.getSwapChainImages( ) [i];
        swapChainImageViews.push_back (context.getVkDevice( ).createImageView (colorSwapChainImageView));
    }
}

DefaultFrameBuffer::DefaultFrameBuffer (const GraphicCore::VulkanDevice& Context,
                                        const GraphicCore::SwapChain&    SwapChain)
    : context (Context)
    , swapChain (SwapChain)
{
    // msaaSamples = context.getDevice ().getMaxUsableSampleCount (); // for now turn off
    /*
    vk::Extent3D swapChainExtent = vk::Extent3D (swapChain.getSwapChainExtent( ), 1);

    for (auto& vkSwapchainImage : SwapChain.getSwapChainImages( ))
    {
        VulkanGame::Ref<GraphicCore::CoreImage> swapchainImage = VulkanGame::CreateRef<GraphicCore::CoreImage> (
            context.getVkDevice( ), vkSwapchainImage, swapChainExtent, swapChain.getColorFormat( ),
            swapChain.GetSupportedImageFlags( ));
        auto renderTarget = fbaFunc (std::move (swapchainImage));
        //renderTarget.Init( );
        frames.emplace_back (std::move (renderTarget));
    }*/
}

void DefaultFrameBuffer::CreateFrameBuffer( )
{
    ImageViewForSwapChainImg( );
    // createColorResources ();// for now turn off
    createDepthResources( );

    vk::FramebufferCreateInfo frameBufferCreateInfo;

    vk::Extent2D swapChainExtent = swapChain.getSwapChainExtent( );

    frameBufferCreateInfo.attachmentCount = renderPass.GetAttachmentCount( );
    frameBufferCreateInfo.renderPass      = renderPass.GetVkRenderPass( );

    frameBufferCreateInfo.width  = swapChainExtent.width;
    frameBufferCreateInfo.height = swapChainExtent.height;
    frameBufferCreateInfo.layers = 1;

    swapChainFramebuffers.resize (swapChainImageViews.size( ));

    for (uint32_t i = 0; i < swapChainImageViews.size( ); ++i)
    {
        // for now no color image as resource if we set color image chnage array resource count to 3 instead of 2
        // std::array<vk::ImageView, 2> attachments = {// colorImage.view,
        //                                            swapChainImageViews [i], depthImage->GetViews( )};

        std::vector<vk::ImageView> attachments;

        for (auto& view : depthImage->GetViews( ))
        {
            attachments.push_back (swapChainImageViews [i]);
            attachments.push_back (view->GetImageView( ));
        }

        assert (attachments.size( ) == frameBufferCreateInfo.attachmentCount);

        frameBufferCreateInfo.pAttachments = attachments.data( );
        swapChainFramebuffers [i]          = context.getVkDevice( ).createFramebuffer (frameBufferCreateInfo);
    }
}

const std::vector<vk::Framebuffer>& DefaultFrameBuffer::getFrameBuffers( ) { return swapChainFramebuffers; }

void DefaultFrameBuffer::destroy( )
{
    if (colorImage) colorImage->destroy( );
    if (depthImage) depthImage->destroy( );

    for (uint32_t i = 0; i < swapChain.getImageCount( ); ++i)
    {
        context.getVkDevice( ).destroyImageView (swapChainImageViews [i]);
        context.getVkDevice( ).destroyFramebuffer (swapChainFramebuffers [i]);
    }
}

void DefaultFrameBuffer::createRenderPass( )
{
    // enable when colorImage will be on
    /*vk::AttachmentDescription colorAttachment = {};
    colorAttachment.format = swapChain.getColorFormat ();
    colorAttachment.samples = msaaSamples;
    colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
    colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
    colorAttachment.finalLayout = vk::ImageLayout::eColorAttachmentOptimal;

    renderPass.setAttachment (colorAttachment);

    vk::AttachmentDescription depthAttachment = {};
    depthAttachment.format = context.depthFormat;
    depthAttachment.samples = msaaSamples;
    depthAttachment.loadOp = vk::AttachmentLoadOp::eClear;;
    depthAttachment.storeOp = vk::AttachmentStoreOp::eDontCare;
    depthAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    depthAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    depthAttachment.initialLayout = vk::ImageLayout::eUndefined;
    depthAttachment.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

    renderPass.setAttachment (depthAttachment);

    vk::AttachmentDescription colorAttachmentResolve = {};
    colorAttachmentResolve.format = swapChain.getColorFormat ();
    colorAttachmentResolve.samples = vk::SampleCountFlagBits::e1;
    colorAttachmentResolve.loadOp = vk::AttachmentLoadOp::eDontCare;
    colorAttachmentResolve.storeOp = vk::AttachmentStoreOp::eStore;
    colorAttachmentResolve.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    colorAttachmentResolve.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    colorAttachmentResolve.initialLayout = vk::ImageLayout::eUndefined;
    colorAttachmentResolve.finalLayout = vk::ImageLayout::ePresentSrcKHR;*/

    vk::AttachmentDescription colorAttachment = { };
    colorAttachment.format                    = swapChain.getColorFormat( );
    colorAttachment.samples                   = msaaSamples;
    colorAttachment.loadOp                    = vk::AttachmentLoadOp::eClear;
    colorAttachment.storeOp                   = vk::AttachmentStoreOp::eStore;
    colorAttachment.stencilLoadOp             = vk::AttachmentLoadOp::eDontCare;
    colorAttachment.stencilStoreOp            = vk::AttachmentStoreOp::eDontCare;
    colorAttachment.initialLayout             = vk::ImageLayout::eUndefined;
    colorAttachment.finalLayout               = vk::ImageLayout::ePresentSrcKHR;

    renderPass.setAttachment (colorAttachment);

    vk::AttachmentDescription depthAttachment = { };
    depthAttachment.format                    = context.depthFormat;
    depthAttachment.samples                   = msaaSamples;
    depthAttachment.loadOp                    = vk::AttachmentLoadOp::eClear;
    depthAttachment.storeOp                   = vk::AttachmentStoreOp::eStore;
    depthAttachment.stencilLoadOp             = vk::AttachmentLoadOp::eClear;
    depthAttachment.stencilStoreOp            = vk::AttachmentStoreOp::eDontCare;
    depthAttachment.initialLayout             = vk::ImageLayout::eUndefined;
    depthAttachment.finalLayout               = vk::ImageLayout::eDepthStencilAttachmentOptimal;

    renderPass.setAttachment (depthAttachment);

    // renderPass.setAttachment (colorAttachmentResolve);

    renderPass.addColorAttachmentReference (0, vk::ImageLayout::eColorAttachmentOptimal);
    renderPass.addDepthAttachmentReference (1, vk::ImageLayout::eDepthStencilAttachmentOptimal);

    // renderPass.addResolveAttachments (2, vk::ImageLayout::eColorAttachmentOptimal);

    renderPass.createSubpass( );

    vk::SubpassDependency dependency = { };
    dependency.srcSubpass            = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass            = 0;
    dependency.srcStageMask          = vk::PipelineStageFlagBits::eBottomOfPipe;
    dependency.srcAccessMask         = vk::AccessFlagBits::eMemoryRead;
    dependency.dstStageMask          = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependency.dstAccessMask   = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;
    dependency.dependencyFlags = vk::DependencyFlagBits::eByRegion;

    renderPass.setSubpassDependency (dependency);

    vk::SubpassDependency dependency2 = { };
    dependency2.srcSubpass            = 0;
    dependency2.dstSubpass            = VK_SUBPASS_EXTERNAL;
    dependency2.srcStageMask          = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependency2.srcAccessMask =
        vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;  // 0
    dependency2.dstStageMask    = vk::PipelineStageFlagBits::eBottomOfPipe;
    dependency2.dstAccessMask   = vk::AccessFlagBits::eMemoryRead;
    dependency2.dependencyFlags = vk::DependencyFlagBits::eByRegion;

    renderPass.setSubpassDependency (dependency2);

    renderPass.createRenderPass( );
}

const vk::RenderPass& DefaultFrameBuffer::getVkRenderPass( ) { return renderPass.GetVkRenderPass( ); }

void DefaultFrameBuffer::createColorResources( )
{
    GraphicCore::ImageContainer imgc = GraphicCore::ImageContainer( );
    imgc.arrayLayer                  = 1;
    imgc.mipLevels                   = 1;
    imgc.TextureExtend               = vk::Extent3D (swapChain.getSwapChainExtent( ), 1);

    colorImage = imageManager.CreateImage (
        imgc, swapChain.getColorFormat( ), vk::ImageTiling::eOptimal,
        vk::ImageUsageFlagBits::eTransientAttachment | vk::ImageUsageFlagBits::eColorAttachment,
        vk::MemoryPropertyFlagBits::eDeviceLocal, msaaSamples);

    VulkanGame::Ref<GraphicCore::CoreImageView> view =
        VulkanGame::CreateRef<GraphicCore::CoreImageView> (context.getVkDevice( ));
    view->CreateImageView (colorImage, vk::ImageViewType::e2D);

    colorImage->GetViews( ).insert (view);
    // imageManager.createImageView (colorImage, vk::ImageAspectFlagBits::eColor, vk::ImageViewType::e2D);
}

void DefaultFrameBuffer::createDepthResources( )
{
    GraphicCore::ImageContainer imgc = GraphicCore::ImageContainer( );

    imgc.arrayLayer    = 1;
    imgc.mipLevels     = 1;
    imgc.TextureExtend = vk::Extent3D (swapChain.getSwapChainExtent( ), 1);

    depthImage = imageManager.CreateImage (
        imgc, context.depthFormat, vk::ImageTiling::eOptimal,
        vk::ImageUsageFlagBits::eDepthStencilAttachment | vk::ImageUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eDeviceLocal, msaaSamples);

    /*
    vk::ImageAspectFlags imgAspectFlags = vk::ImageAspectFlagBits::eDepth;
    // Stencil aspect should only be set on depth + stencil formats
    // (VK_FORMAT_D16_UNORM_S8_UINT..VK_FORMAT_D32_SFLOAT_S8_UINT
    if (depthFormat >= vk::Format::eD16UnormS8Uint) { imgAspectFlags |= vk::ImageAspectFlagBits::eStencil; }
    */

    VulkanGame::Ref<GraphicCore::CoreImageView> view =
        VulkanGame::CreateRef<GraphicCore::CoreImageView> (context.getVkDevice( ));
    view->CreateImageView (depthImage, vk::ImageViewType::e2D);

    depthImage->GetViews( ).insert (view);
}