#ifndef RENDER_PASS_HPP
#define RENDER_PASS_HPP

#include <vulkan/vulkan.hpp>

namespace GameCore {
    class RenderPass {
        // Global render pass for frame buffer writes
        vk::RenderPass renderPass;

        std::vector<vk::AttachmentDescription> attachments;
        std::vector<vk::SubpassDependency>     subpassDependencies;
        std::vector<vk::SubpassDescription>    subpasses;

        // Only one depth attachment, so put it first in the references
        vk::AttachmentReference depthReference;
        vk::AttachmentReference resolveAttachments;

        std::vector<vk::AttachmentReference> colorAttachmentReferences;

        uint32_t attachmentCount = 0;

       public:
        void destroy (const vk::Device& device) const;

        void createRenderPass (const vk::Device& device);
        void createSubpass( );

        void setAttachment (vk::AttachmentDescriptionFlags flags_          = vk::AttachmentDescriptionFlags( ),
                            vk::Format                     format_         = vk::Format::eUndefined,
                            vk::SampleCountFlagBits        samples_        = vk::SampleCountFlagBits::e1,
                            vk::AttachmentLoadOp           loadOp_         = vk::AttachmentLoadOp::eLoad,
                            vk::AttachmentStoreOp          storeOp_        = vk::AttachmentStoreOp::eStore,
                            vk::AttachmentLoadOp           stencilLoadOp_  = vk::AttachmentLoadOp::eLoad,
                            vk::AttachmentStoreOp          stencilStoreOp_ = vk::AttachmentStoreOp::eStore,
                            vk::ImageLayout                initialLayout_  = vk::ImageLayout::eUndefined,
                            vk::ImageLayout                finalLayout_    = vk::ImageLayout::eUndefined);

        void setAttachment (const vk::AttachmentDescription& attachment);

        void setSubpassDependency (uint32_t srcSubpass_ = 0, uint32_t dstSubpass_ = 0,
                                   vk::PipelineStageFlags srcStageMask_    = vk::PipelineStageFlags( ),
                                   vk::PipelineStageFlags dstStageMask_    = vk::PipelineStageFlags( ),
                                   vk::AccessFlags        srcAccessMask_   = vk::AccessFlags( ),
                                   vk::AccessFlags        dstAccessMask_   = vk::AccessFlags( ),
                                   vk::DependencyFlags    dependencyFlags_ = vk::DependencyFlags( ));

        void setSubpassDependency (const vk::SubpassDependency& subpassDependency);

        void addColorAttachmentReference (const uint32_t& attachment, const vk::ImageLayout& layout);

        void addDepthAttachmentReference (const uint32_t& attachment, const vk::ImageLayout& layout);

        void addResolveAttachments (const uint32_t& attachment, const vk::ImageLayout& layout);

        const vk::RenderPass& GetVkRenderPass( ) const;
        const uint32_t&       GetAttachmentCount( ) const;

        // std::vector<vk::AttachmentDescription> GetAttachment();
    };
}  // namespace GameCore

#endif  // RENDER_PASS_HPP
