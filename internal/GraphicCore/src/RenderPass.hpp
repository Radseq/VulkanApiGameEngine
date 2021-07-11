#ifndef RENDER_PASS_HPP
#define RENDER_PASS_HPP

#include <FrameBufferAttachment.hpp>
#include <vkBasicModelsContainer.hpp>

#include "pch.h"

namespace GraphicCore
{
    class RenderPass
    {
        const vk::Device& m_Device;

        // Global render pass for frame buffer writes
        vk::RenderPass renderPass;

        uint32_t attachmentCount {0};

        size_t m_SubpassCount {1};

        // Store attachments for every subpass
        std::vector<std::vector<vk::AttachmentReference>> m_InputAttachments;
        std::vector<std::vector<vk::AttachmentReference>> m_ColorAttachments;
        std::vector<std::vector<vk::AttachmentReference>> m_DepthStencilAttachments;

       public:
        void destroy( );

        void createRenderPass( );
        void createSubpass( );

        const vk::RenderPass& GetVkRenderPass( ) const;
        const uint32_t&       GetAttachmentCount( ) const;


        RenderPass (const vk::Device& device, const std::vector<vkBasicModels::AttachmentModel>& Attachemnts,
                    const std::vector<vkBasicModels::LoadStoreInfo>& load_store_infos,
                    const std::vector<vkBasicModels::SubpassInfo>&   Subpasses);
    };
}  // namespace GraphicCore

#endif  // RENDER_PASS_HPP
