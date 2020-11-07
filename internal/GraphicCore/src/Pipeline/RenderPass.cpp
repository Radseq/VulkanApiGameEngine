#include "RenderPass.hpp"

namespace GraphicCore {
    void RenderPass::destroy (const vk::Device& device) const { device.destroyRenderPass (renderPass); }

    void RenderPass::createRenderPass (const vk::Device& device) {
        if (renderPass) { device.destroyRenderPass (renderPass); }

        vk::RenderPassCreateInfo renderPassInfo;
        renderPassInfo.attachmentCount = attachmentCount = uint32_t (attachments.size( ));
        renderPassInfo.pAttachments                      = attachments.data( );
        renderPassInfo.subpassCount                      = uint32_t (subpasses.size( ));
        renderPassInfo.pSubpasses                        = subpasses.data( );
        renderPassInfo.dependencyCount                   = uint32_t (subpassDependencies.size( ));
        renderPassInfo.pDependencies                     = subpassDependencies.data( );
        renderPass                                       = device.createRenderPass (renderPassInfo);
    }

    void RenderPass::createSubpass( ) {
        /*
        const vk::SubpassDescription subpass{
                {},
                vk::PipelineBindPoint::eGraphics,
                // Input attachment references
                0,
                nullptr,
                // Color / resolve attachment references
                uint32_t(colorAttachmentReferences.size()),
                colorAttachmentReferences.data(),
                nullptr,
                // Depth stecil attachment reference,
                (depthReference == vk::AttachmentReference() ? nullptr : &depthReference),
                // Preserve attachments
                0, nullptr
        };*/

        vk::SubpassDescription subpass  = { };
        subpass.pipelineBindPoint       = vk::PipelineBindPoint::eGraphics;
        subpass.colorAttachmentCount    = uint32_t (colorAttachmentReferences.size( ));  // should be 1
        subpass.pColorAttachments       = colorAttachmentReferences.data( );
        subpass.pDepthStencilAttachment = (depthReference == vk::AttachmentReference( ) ? nullptr : &depthReference);
        subpass.pResolveAttachments =
            (resolveAttachments == vk::AttachmentReference( ) ? nullptr : &resolveAttachments);

        subpasses.push_back (subpass);
    }

    void RenderPass::setAttachment (const vk::AttachmentDescriptionFlags flags_, const vk::Format format_,
                                    const vk::SampleCountFlagBits samples_, const vk::AttachmentLoadOp loadOp_,
                                    const vk::AttachmentStoreOp storeOp_, const vk::AttachmentLoadOp stencilLoadOp_,
                                    const vk::AttachmentStoreOp stencilStoreOp_, const vk::ImageLayout initialLayout_,
                                    const vk::ImageLayout finalLayout_) {
        attachments.push_back (vk::AttachmentDescription {flags_, format_, samples_, loadOp_, storeOp_, stencilLoadOp_,
                                                          stencilStoreOp_, initialLayout_, finalLayout_});
    }

    void RenderPass::setAttachment (const vk::AttachmentDescription& attachment) {
        attachments.push_back ({vk::AttachmentDescription {attachment}});
    }

    // maybe will be less fuck up later when see what needeed to be usage
    void RenderPass::setSubpassDependency (const uint32_t srcSubpass, const uint32_t dstSubpass,
                                           const vk::PipelineStageFlags srcStageMask,
                                           const vk::PipelineStageFlags dstStageMask,
                                           const vk::AccessFlags srcAccessMask, const vk::AccessFlags dstAccessMask,
                                           const vk::DependencyFlags dependencyFlags) {
        subpassDependencies.push_back (vk::SubpassDependency {srcSubpass, dstSubpass, srcStageMask, dstStageMask,
                                                              srcAccessMask, dstAccessMask, dependencyFlags});
    }

    void RenderPass::setSubpassDependency (const vk::SubpassDependency& subpassDependency) {
        subpassDependencies.push_back (vk::SubpassDependency {subpassDependency});
    }

    void RenderPass::addColorAttachmentReference (const uint32_t& attachment, const vk::ImageLayout& layout) {
        colorAttachmentReferences.push_back (vk::AttachmentReference {attachment, layout});
    }

    void RenderPass::addDepthAttachmentReference (const uint32_t& attachment, const vk::ImageLayout& layout) {
        depthReference = vk::AttachmentReference {attachment, layout};
    }

    void RenderPass::addResolveAttachments (const uint32_t& attachment, const vk::ImageLayout& layout) {
        resolveAttachments = vk::AttachmentReference {attachment, layout};
    }

    const vk::RenderPass& RenderPass::GetVkRenderPass( ) const { return renderPass; }
    const uint32_t&       RenderPass::GetAttachmentCount( ) const { return attachmentCount; }

    /*std::vector<vk::AttachmentDescription> RenderPass::GetAttachment()
    {
            return attachments;
    }*/
}  // namespace GraphicCore
