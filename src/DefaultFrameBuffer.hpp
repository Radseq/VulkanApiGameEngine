#ifndef DEFAULT_FRAME_BUFFER_HPP
#define DEFAULT_FRAME_BUFFER_HPP

#include <CoreImageView.hpp>
#include <FrameBufferAttachment.hpp>
#include <ImageManager.hpp>
#include <SwapChain.hpp>

#include "pch.h"

class DefaultFrameBuffer
{
    std::vector<vk::ImageView>   swapChainImageViews;
    std::vector<vk::Framebuffer> swapChainFramebuffers;
    /*
    GraphicCore::CoreImage colorImage{};
    GraphicCore::CoreImageView colorImageView{};

    GraphicCore::CoreImage depthImage{};
    GraphicCore::CoreImageView depthImageView{};*/

    VulkanGame::Ref<GraphicCore::CoreImage> colorImage;
    VulkanGame::Ref<GraphicCore::CoreImage> depthImage;

    vk::SampleCountFlagBits msaaSamples = vk::SampleCountFlagBits::e1;

    const GraphicCore::VulkanDevice& context;
    const GraphicCore::SwapChain&    swapChain;
    GraphicCore::ImageManager        imageManager {context};

    void ImageViewForSwapChainImg( );

    void createColorResources( );
    void createDepthResources( );

    GraphicCore::RenderPass renderPass {context.getVkDevice( )};

    std::vector<GraphicCore::FrameBufferAttachment> frames;

    GraphicCore::FrameBufferAttachment::CreateFunc fbaFunc =
        GraphicCore::FrameBufferAttachment::DEFAULT_CREATE_FUNC;

   public:
    DefaultFrameBuffer (const GraphicCore::VulkanDevice& Context, const GraphicCore::SwapChain& SwapChain);
    void                                CreateFrameBuffer( );
    void                                createRenderPass( );
    const std::vector<vk::Framebuffer>& getFrameBuffers( );
    const vk::RenderPass&               getVkRenderPass( );
    void                                destroy( );
};

#endif  // DEFAULT_FRAME_BUFFER_HPP
