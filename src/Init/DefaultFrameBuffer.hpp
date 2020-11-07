#ifndef DEFAULT_FRAME_BUFFER_HPP
#define DEFAULT_FRAME_BUFFER_HPP

#include <Framebuffer\SwapChain.hpp>
#include <Image\CoreImage.hpp>
#include <Image\CoreImageView.hpp>
#include <Image\ImageManager.hpp>
#include <vulkan/vulkan.hpp>

class DefaultFrameBuffer {
    std::vector<vk::ImageView>   swapChainImageViews;
    std::vector<vk::Framebuffer> swapChainFramebuffers;
    /*
    GraphicCore::CoreImage colorImage{};
    GraphicCore::CoreImageView colorImageView{};

    GraphicCore::CoreImage depthImage{};
    GraphicCore::CoreImageView depthImageView{};*/

    GraphicCore::Image colorImage;
    GraphicCore::Image depthImage;

    vk::SampleCountFlagBits msaaSamples = vk::SampleCountFlagBits::e1;

    const GraphicCore::VulkanDevice& context;
    const GraphicCore::SwapChain&    swapChain;
    GraphicCore::ImageManager        imageManager {context};

    void ImageViewForSwapChainImg( );

    void createColorResources( );
    void createDepthResources( );

    GraphicCore::RenderPass renderPass;

   public:
    DefaultFrameBuffer (const GraphicCore::VulkanDevice& Context, const GraphicCore::SwapChain& SwapChain);
    void                                CreateFrameBuffer( );
    void                                createRenderPass( );
    const std::vector<vk::Framebuffer>& getFrameBuffers( );
    const vk::RenderPass&               getVkRenderPass( );
    void                                destroy( );
};

#endif  // DEFAULT_FRAME_BUFFER_HPP
