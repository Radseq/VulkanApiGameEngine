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
        std::array<vk::ImageView, 2> attachments = {// colorImage.view,
                                                    swapChainImageViews [i], depthImage->view};

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
    ;
    depthAttachment.storeOp        = vk::AttachmentStoreOp::eStore;
    depthAttachment.stencilLoadOp  = vk::AttachmentLoadOp::eClear;
    depthAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    depthAttachment.initialLayout  = vk::ImageLayout::eUndefined;
    depthAttachment.finalLayout    = vk::ImageLayout::eDepthStencilAttachmentOptimal;

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

    renderPass.createRenderPass (context.getVkDevice( ));
}

const vk::RenderPass& DefaultFrameBuffer::getVkRenderPass( ) { return renderPass.GetVkRenderPass( ); }

void DefaultFrameBuffer::createColorResources( )
{
    vk::Format   colorFormat     = swapChain.getColorFormat( );
    vk::Extent2D swapChainExtent = swapChain.getSwapChainExtent( );

    vk::ImageCreateInfo imageInfo = { };
    imageInfo.imageType           = vk::ImageType::e2D;
    imageInfo.extent.width        = swapChainExtent.width;
    imageInfo.extent.height       = swapChainExtent.height;
    imageInfo.extent.depth        = 1;
    imageInfo.mipLevels           = 1;
    imageInfo.arrayLayers         = 1;
    imageInfo.format              = colorFormat;
    imageInfo.tiling              = vk::ImageTiling::eOptimal;
    imageInfo.initialLayout       = vk::ImageLayout::eUndefined;
    imageInfo.usage       = vk::ImageUsageFlagBits::eTransientAttachment | vk::ImageUsageFlagBits::eColorAttachment;
    imageInfo.samples     = msaaSamples;
    imageInfo.sharingMode = vk::SharingMode::eExclusive;

    colorImage = imageManager.CreateImage (imageInfo);
    imageManager.createImageView (colorImage, vk::ImageAspectFlagBits::eColor, vk::ImageViewType::e2D);
}

void DefaultFrameBuffer::createDepthResources( )
{
    vk::Format   depthFormat     = context.depthFormat;
    vk::Extent2D swapChainExtent = swapChain.getSwapChainExtent( );

    vk::ImageCreateInfo imageInfo = { };
    imageInfo.imageType           = vk::ImageType::e2D;
    imageInfo.extent.width        = swapChainExtent.width;
    imageInfo.extent.height       = swapChainExtent.height;
    imageInfo.extent.depth        = 1;
    imageInfo.mipLevels           = 1;
    imageInfo.arrayLayers         = 1;
    imageInfo.format              = depthFormat;
    imageInfo.tiling              = vk::ImageTiling::eOptimal;
    imageInfo.initialLayout       = vk::ImageLayout::eUndefined;
    imageInfo.usage       = vk::ImageUsageFlagBits::eDepthStencilAttachment | vk::ImageUsageFlagBits::eTransferSrc;
    imageInfo.samples     = msaaSamples;
    imageInfo.sharingMode = vk::SharingMode::eExclusive;

    depthImage = imageManager.CreateImage (imageInfo);

    vk::ImageAspectFlags imgAspectFlags = vk::ImageAspectFlagBits::eDepth;
    // Stencil aspect should only be set on depth + stencil formats
    // (VK_FORMAT_D16_UNORM_S8_UINT..VK_FORMAT_D32_SFLOAT_S8_UINT
    if (depthFormat >= vk::Format::eD16UnormS8Uint) { imgAspectFlags |= vk::ImageAspectFlagBits::eStencil; }

    imageManager.createImageView (depthImage, imgAspectFlags, vk::ImageViewType::e2D);
}