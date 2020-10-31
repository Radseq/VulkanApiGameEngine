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
    GameCore::CoreImage colorImage{};
    GameCore::CoreImageView colorImageView{};

    GameCore::CoreImage depthImage{};
    GameCore::CoreImageView depthImageView{};*/

    GameCore::Image colorImage;
    GameCore::Image depthImage;

    vk::SampleCountFlagBits msaaSamples = vk::SampleCountFlagBits::e1;

    const GameCore::VulkanDevice& context;
    const GameCore::SwapChain&    swapChain;
    GameCore::ImageManager        imageManager {context};

    void ImageViewForSwapChainImg( );

    void createColorResources( );
    void createDepthResources( );

    GameCore::RenderPass renderPass;

   public:
    DefaultFrameBuffer (const GameCore::VulkanDevice& Context, const GameCore::SwapChain& SwapChain);
    void                                CreateFrameBuffer( );
    void                                createRenderPass( );
    const std::vector<vk::Framebuffer>& getFrameBuffers( );
    const vk::RenderPass&               getVkRenderPass( );
    void                                destroy( );
};

#endif  // DEFAULT_FRAME_BUFFER_HPP
