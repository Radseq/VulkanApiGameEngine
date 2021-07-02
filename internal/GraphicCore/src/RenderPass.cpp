#include "pch.h"
#include "RenderPass.hpp"

namespace GraphicCore
{
    void RenderPass::destroy( ) { m_Device.destroyRenderPass (renderPass); }

    void RenderPass::createRenderPass( )
    {
        if (renderPass) { m_Device.destroyRenderPass (renderPass); }

        vk::RenderPassCreateInfo renderPassInfo;
        renderPassInfo.attachmentCount = attachmentCount = uint32_t (attachments.size( ));
        renderPassInfo.pAttachments                      = attachments.data( );
        renderPassInfo.subpassCount                      = uint32_t (subpasses.size( ));
        renderPassInfo.pSubpasses                        = subpasses.data( );
        renderPassInfo.dependencyCount                   = uint32_t (subpassDependencies.size( ));
        renderPassInfo.pDependencies                     = subpassDependencies.data( );
        renderPass                                       = m_Device.createRenderPass (renderPassInfo);
    }

    void RenderPass::createSubpass( )
    {
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
                                    const vk::ImageLayout finalLayout_)
    {
        attachments.push_back (vk::AttachmentDescription {flags_, format_, samples_, loadOp_, storeOp_, stencilLoadOp_,
                                                          stencilStoreOp_, initialLayout_, finalLayout_});
    }

    void RenderPass::setAttachment (const vk::AttachmentDescription& attachment)
    {
        attachments.push_back ({vk::AttachmentDescription {attachment}});
    }

    // maybe will be less fuck up later when see what needeed to be usage
    void RenderPass::setSubpassDependency (const uint32_t srcSubpass, const uint32_t dstSubpass,
                                           const vk::PipelineStageFlags srcStageMask,
                                           const vk::PipelineStageFlags dstStageMask,
                                           const vk::AccessFlags srcAccessMask, const vk::AccessFlags dstAccessMask,
                                           const vk::DependencyFlags dependencyFlags)
    {
        subpassDependencies.push_back (vk::SubpassDependency {srcSubpass, dstSubpass, srcStageMask, dstStageMask,
                                                              srcAccessMask, dstAccessMask, dependencyFlags});
    }

    void RenderPass::setSubpassDependency (const vk::SubpassDependency& subpassDependency)
    {
        subpassDependencies.push_back (vk::SubpassDependency {subpassDependency});
    }

    void RenderPass::addColorAttachmentReference (const uint32_t& attachment, const vk::ImageLayout& layout)
    {
        colorAttachmentReferences.push_back (vk::AttachmentReference {attachment, layout});
    }

    void RenderPass::addDepthAttachmentReference (const uint32_t& attachment, const vk::ImageLayout& layout)
    {
        depthReference = vk::AttachmentReference {attachment, layout};
    }

    void RenderPass::addResolveAttachments (const uint32_t& attachment, const vk::ImageLayout& layout)
    {
        resolveAttachments = vk::AttachmentReference {attachment, layout};
    }

    const vk::RenderPass& RenderPass::GetVkRenderPass( ) const { return renderPass; }
    const uint32_t&       RenderPass::GetAttachmentCount( ) const { return attachmentCount; }

    /*std::vector<vk::AttachmentDescription> RenderPass::GetAttachment()
    {
            return attachments;
    }*/

    RenderPass::RenderPass (const vk::Device&                                           device,
                            const std::vector<vkBasicModels::AttachmentModel>& attachemnts,
                            const std::vector<vkBasicModels::LoadStoreInfo>&   load_store_infos,
                            const std::vector<vkBasicModels::SubpassInfo>&     subpasses)
        : m_Device {device}
        , m_SubpassCount {std::max<size_t> (1, subpasses.size( ))}
        ,  // At least 1 subpass
        m_InputAttachments {m_SubpassCount}
        , m_ColorAttachments {m_SubpassCount}
        , m_DepthStencilAttachments {m_SubpassCount}
    {
        uint32_t depth_stencil_attachment {VK_ATTACHMENT_UNUSED};

        std::vector<vk::AttachmentDescription> attachment_descriptions;

        for (uint32_t i = 0U; i < attachments.size( ); ++i)
        {
            vk::AttachmentDescription attachment { };

            attachment.format      = attachments [i].format;
            attachment.samples     = attachments [i].samples;
            attachment.finalLayout = vk::ImageLayout::eColorAttachmentOptimal;

            if (i < load_store_infos.size( ))
            {
                attachment.loadOp         = load_store_infos [i].load_op;
                attachment.storeOp        = load_store_infos [i].store_op;
                attachment.stencilLoadOp  = load_store_infos [i].load_op;
                attachment.stencilStoreOp = load_store_infos [i].store_op;
            }

            if (vkHelper::HasDepthStencilFormat (attachment.format))
            {
                depth_stencil_attachment = i;
                attachment.finalLayout   = vk::ImageLayout::eDepthStencilAttachmentOptimal;
            }

            attachment_descriptions.push_back (std::move (attachment));
        }

        std::vector<vk::SubpassDescription> subpass_descriptions;
        subpass_descriptions.reserve (m_SubpassCount);

        for (size_t i = 0; i < subpasses.size( ); ++i)
        {
            const auto& subpass = subpasses [i];

            // Fill color/depth attachments references
            for (auto o_attachment : subpass.OutputAttachments)
            {
                if (o_attachment != depth_stencil_attachment)
                { m_ColorAttachments [i].push_back ({o_attachment, vk::ImageLayout::eColorAttachmentOptimal}); }
            }

            // Fill input attachments references
            for (const auto& i_attachment : subpass.InputAttachments)
            {
                if (vkHelper::HasDepthStencilFormat (attachment_descriptions [i_attachment].format))
                { m_InputAttachments [i].push_back ({i_attachment, vk::ImageLayout::eDepthStencilReadOnlyOptimal}); }
                else
                {
                    m_InputAttachments [i].push_back ({i_attachment, vk::ImageLayout::eShaderReadOnlyOptimal});
                }
            }

            if (depth_stencil_attachment != VK_ATTACHMENT_UNUSED)
            {
                m_DepthStencilAttachments [i].push_back (
                    {depth_stencil_attachment, vk::ImageLayout::eDepthStencilAttachmentOptimal});
            }
        }

        for (size_t i = 0; i < subpasses.size( ); ++i)
        {
            vk::SubpassDescription subpass_description { };
            subpass_description.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;

            subpass_description.pInputAttachments =
                m_InputAttachments [i].empty( ) ? nullptr : m_InputAttachments [i].data( );
            subpass_description.inputAttachmentCount = GraphicCore::Util::toUint32t (m_InputAttachments [i].size( ));

            subpass_description.pColorAttachments =
                m_ColorAttachments [i].empty( ) ? nullptr : m_ColorAttachments [i].data( );
            subpass_description.colorAttachmentCount = GraphicCore::Util::toUint32t (m_ColorAttachments [i].size( ));

            subpass_description.pDepthStencilAttachment =
                m_DepthStencilAttachments [i].empty( ) ? nullptr : m_DepthStencilAttachments [i].data( );

            subpass_descriptions.push_back (subpass_description);
        }

        // Default subpass
        if (subpasses.empty( ))
        {
            vk::SubpassDescription subpass_description { };
            subpass_description.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;

            for (uint32_t k = 0U; k < attachment_descriptions.size( ); ++k)
            {
                if (k == depth_stencil_attachment) { continue; }

                m_ColorAttachments [0].push_back ({k, vk::ImageLayout::eGeneral});
            }

            subpass_description.pColorAttachments = m_ColorAttachments [0].data( );

            if (depth_stencil_attachment != VK_ATTACHMENT_UNUSED)
            {
                m_DepthStencilAttachments [0].push_back (
                    {depth_stencil_attachment, vk::ImageLayout::eDepthStencilAttachmentOptimal});

                subpass_description.pDepthStencilAttachment = m_DepthStencilAttachments [0].data( );
            }

            subpass_descriptions.push_back (subpass_description);
        }

        // Make the initial layout same as in the first subpass using that attachment
        for (const auto& subpass : subpass_descriptions)
        {
            for (uint32_t k = 0U; k < subpass.colorAttachmentCount; ++k)
            {
                auto reference = subpass.pColorAttachments [k];
                // Set it only if not defined yet
                if (attachment_descriptions [reference.attachment].initialLayout == vk::ImageLayout::eUndefined)
                { attachment_descriptions [reference.attachment].initialLayout = reference.layout; }
            }

            for (uint32_t k = 0U; k < subpass.inputAttachmentCount; ++k)
            {
                auto reference = subpass.pInputAttachments [k];
                // Set it only if not defined yet
                if (attachment_descriptions [reference.attachment].initialLayout == vk::ImageLayout::eUndefined)
                { attachment_descriptions [reference.attachment].initialLayout = reference.layout; }
            }

            if (subpass.pDepthStencilAttachment)
            {
                auto reference = *subpass.pDepthStencilAttachment;
                // Set it only if not defined yet
                if (attachment_descriptions [reference.attachment].initialLayout == vk::ImageLayout::eUndefined)
                { attachment_descriptions [reference.attachment].initialLayout = reference.layout; }
            }
        }

        // Make the final layout same as the last subpass layout
        {
            auto& subpass = subpass_descriptions.back( );

            for (uint32_t k = 0U; k < subpass.colorAttachmentCount; ++k)
            {
                const auto& reference = subpass.pColorAttachments [k];

                attachment_descriptions [reference.attachment].finalLayout = reference.layout;
            }

            for (uint32_t k = 0U; k < subpass.inputAttachmentCount; ++k)
            {
                const auto& reference = subpass.pInputAttachments [k];

                attachment_descriptions [reference.attachment].finalLayout = reference.layout;

                // Do not use depth attachment if used as input
                if (reference.attachment == depth_stencil_attachment) { subpass.pDepthStencilAttachment = nullptr; }
            }

            if (subpass.pDepthStencilAttachment)
            {
                const auto& reference = *subpass.pDepthStencilAttachment;

                attachment_descriptions [reference.attachment].finalLayout = reference.layout;
            }
        }

        // Set subpass dependencies
        std::vector<vk::SubpassDependency> dependencies (m_SubpassCount - 1);

        if (m_SubpassCount > 1)
        {
            for (uint32_t i = 0; i < dependencies.size( ); ++i)
            {
                // Transition input attachments from color attachment to shader read
                dependencies [i].srcSubpass      = i;
                dependencies [i].dstSubpass      = i + 1;
                dependencies [i].srcStageMask    = vk::PipelineStageFlagBits::eColorAttachmentOutput;
                dependencies [i].dstStageMask    = vk::PipelineStageFlagBits::eFragmentShader;
                dependencies [i].srcAccessMask   = vk::AccessFlagBits::eColorAttachmentWrite;
                dependencies [i].dstAccessMask   = vk::AccessFlagBits::eInputAttachmentRead;
                dependencies [i].dependencyFlags = vk::DependencyFlagBits::eByRegion;
            }
        }

        // Create render pass
        vk::RenderPassCreateInfo create_info;

        create_info.attachmentCount = GraphicCore::Util::toUint32t (attachment_descriptions.size( ));
        create_info.pAttachments    = attachment_descriptions.data( );
        create_info.subpassCount    = GraphicCore::Util::toUint32t (m_SubpassCount);
        create_info.pSubpasses      = subpass_descriptions.data( );
        create_info.dependencyCount = GraphicCore::Util::toUint32t (dependencies.size( ));
        create_info.pDependencies   = dependencies.data( );

        // renderPass = m_Device.createRenderPass (create_info);

    }  // namespace vkb
}  // namespace GraphicCore
