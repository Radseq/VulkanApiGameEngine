#ifndef VK_BASIC_MODELS_CONTAINERS_HPP
#define VK_BASIC_MODELS_CONTAINERS_HPP

#include "pch.h"

namespace GraphicCore
{
    namespace vkBasicModels
    {
        struct LoadStoreInfo
        {
            vk::AttachmentLoadOp  load_op  = vk::AttachmentLoadOp::eClear;
            vk::AttachmentStoreOp store_op = vk::AttachmentStoreOp::eStore;
        };

        struct SubpassInfo
        {
            std::vector<uint32_t> InputAttachments;
            std::vector<uint32_t> OutputAttachments;
        };

        struct AttachmentModel
        {
            vk::Format              format {vk::Format::eUndefined};
            vk::SampleCountFlagBits samples {vk::SampleCountFlagBits::e1};
            vk::ImageUsageFlags     usage {vk::ImageUsageFlagBits::eSampled};

            AttachmentModel( ) = default;

            AttachmentModel (const vk::Format &format, const vk::SampleCountFlagBits &samples,
                             const vk::ImageUsageFlags &usage)
                : format {format}
                , samples {samples}
                , usage {usage}
            {
            }
        };
    }  // namespace vkBasicModels
}  // namespace GraphicCore
#endif  // VK_BASIC_MODELS_CONTAINERS_HPP