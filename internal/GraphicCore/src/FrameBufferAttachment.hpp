#ifndef FRAME_BUFFER_ATTACHMENT_HPP
#define FRAME_BUFFER_ATTACHMENT_HPP

#include <set>
#include <vkBasicModelsContainer.hpp>

#include "CoreImage.hpp"
#include "CoreImageView.hpp"
#include "pch.h"

namespace GraphicCore
{
    /**
     * @brief FrameBufferAttachment contains three vectors for: GraphicCore::CoreImage, GraphicCore::ImageView and
     * AttachmentModel. The first two are Vulkan images and corresponding image views respectively. AttachmentModel (s)
     * contain a description of the images, which has two main purposes:
     * - RenderPass creation only needs a list of AttachmentModel (s), not the actual images, so we keep
     *   the minimum amount of information necessary
     * - Creation of a FrameBufferAttachment becomes simpler, because the caller can just ask for some
     *   AttachmentModel (s) without having to create the images
     */

    class FrameBufferAttachment
    {
        const VulkanDevice &m_LogicalDevice;

        vk::Extent2D extent { };

        std::vector<std::shared_ptr<CoreImage>>     m_Images;
        std::vector<std::shared_ptr<CoreImageView>> m_Views;
        std::vector<vkBasicModels::AttachmentModel> m_Attachments;

        /// By default there are no input attachments
        std::vector<uint32_t> m_InputAttachments = { };

        /// By default the output attachments is attachment 0
        std::vector<uint32_t> m_OutputAttachments = {0};

       public:
        using CreateFunc = std::function<FrameBufferAttachment (std::shared_ptr<CoreImage> &&)>;

        static const CreateFunc DEFAULT_CREATE_FUNC;

        FrameBufferAttachment (std::vector<std::shared_ptr<CoreImage>> &&images);
        FrameBufferAttachment (const FrameBufferAttachment &) = delete;
        FrameBufferAttachment (FrameBufferAttachment &&)      = default;


        // vk::ResultValueType<void>::type Init( );

        const vk::Extent2D &                               GetExtent( ) const;
        const std::vector<std::shared_ptr<CoreImageView>>  GetViews( ) const;
        const std::vector<vkBasicModels::AttachmentModel> &GetAttachments( ) const;
        const std::vector<uint32_t> &                      GetOutputAttachments( ) const;
        const std::vector<uint32_t> &                      GetInputAttachments( ) const;

        /**
         * @brief Sets the current input attachments overwriting the current ones
         *        Should be set before beginning the render pass and before starting a new subpass
         * @param input Set of attachment reference number to use as input
         */

        void SetInputAttachments (std::vector<uint32_t> &input);

        /**
         * @brief Sets the current output attachments overwriting the current ones
         *        Should be set before beginning the render pass and before starting a new subpass
         * @param output Set of attachment reference number to use as output
         */

        void SetOutputAttachments (std::vector<uint32_t> &output);
    };
}  // namespace GraphicCore

#endif /* FRAME_BUFFER_ATTACHMENT_HPP */