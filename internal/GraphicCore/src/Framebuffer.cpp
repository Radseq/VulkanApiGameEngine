#include "pch.h"
#include "Framebuffer.hpp"

namespace GraphicCore
{
    Framebuffer::Framebuffer (const vk::Device &Device, const FrameBufferAttachment &FBA, const RenderPass &RenderPass)
        : m_Device {Device}
    {
        auto &extent = FBA.GetExtent( );

        std::vector<vk::ImageView> attachments;

        for (auto &view : FBA.GetViews( )) { attachments.emplace_back (view->GetImageView( )); }

        vk::FramebufferCreateInfo createInfo;

        createInfo.renderPass      = RenderPass.GetVkRenderPass( );
        createInfo.attachmentCount = static_cast<uint32_t> (attachments.size( ));
        createInfo.pAttachments    = attachments.data( );
        createInfo.width           = extent.width;
        createInfo.height          = extent.height;
        createInfo.layers          = 1;

        m_Framebuffer = m_Device.createFramebuffer (createInfo);
    }

    Framebuffer::Framebuffer (Framebuffer &&other) noexcept
        : m_Device {other.m_Device}
        , m_Framebuffer {other.m_Framebuffer}
    {
        other.m_Framebuffer = nullptr;
    }

    Framebuffer::~Framebuffer( ) { m_Device.destroyFramebuffer (m_Framebuffer); }

    vk::Framebuffer Framebuffer::GetVkFrameBuffer( ) const { return m_Framebuffer; }
}  // namespace GraphicCore
