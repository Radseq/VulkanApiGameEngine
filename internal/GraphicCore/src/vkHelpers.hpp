/*
 * Assorted commonly used Vulkan helper functions
 *
 * Copyright (C) 2016 by Sascha Willems - www.saschawillems.de
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VK_HELPERS_HPP
#define VK_HELPERS_HPP

#include "pch.h"

namespace GraphicCore
{
    namespace vkHelper
    {
        struct CompareExtent2D
        {
            bool operator( ) (const vk::Extent2D& lhs, const vk::Extent2D& rhs) const
            {
                return !(lhs.width == rhs.width && lhs.height == rhs.height) &&
                       (lhs.width < rhs.width && lhs.height < rhs.height);
            }
        };

        constexpr bool HasDepthFormat (const vk::Format& format)
        {
            std::array<vk::Format, 6> formats = {
                vk::Format::eD16Unorm,       vk::Format::eX8D24UnormPack32, vk::Format::eD32Sfloat,
                vk::Format::eD16UnormS8Uint, vk::Format::eD24UnormS8Uint,   vk::Format::eD32SfloatS8Uint,
            };
            return std::find (formats.begin( ), formats.end( ), format) != std::end (formats);
        }

        constexpr bool HasStencil (const vk::Format& format)
        {
            std::array<vk::Format, 4> formats = {
                vk::Format::eS8Uint,
                vk::Format::eD16UnormS8Uint,
                vk::Format::eD24UnormS8Uint,
                vk::Format::eD32SfloatS8Uint,
            };
            return std::find (formats.begin( ), formats.end( ), format) != std::end (formats);
        }

        constexpr bool HasDepthStencilFormat (const vk::Format& format)
        {
            return (HasDepthFormat (format) || HasStencil (format));
        }

        constexpr vk::ColorComponentFlags fullColorWriteMask( )
        {
            return vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB |
                   vk ::ColorComponentFlagBits::eA;
        }

        inline vk::Viewport viewport (float width, float height, float minDepth = 0, float maxDepth = 1)
        {
            vk::Viewport viewport;
            viewport.width    = width;
            viewport.height   = height;
            viewport.minDepth = minDepth;
            viewport.maxDepth = maxDepth;
            return viewport;
        }

        inline vk::Viewport viewport (const vk::Extent2D& size, float minDepth = 0, float maxDepth = 1)
        {
            return viewport (static_cast<float> (size.width), static_cast<float> (size.height), minDepth, maxDepth);
        }

        inline vk::Rect2D rect2D (uint32_t width, uint32_t height, int32_t offsetX = 0, int32_t offsetY = 0)
        {
            vk::Rect2D rect2D;
            rect2D.extent.width  = width;
            rect2D.extent.height = height;
            rect2D.offset.x      = offsetX;
            rect2D.offset.y      = offsetY;
            return rect2D;
        }

        inline vk::Rect2D rect2D (const vk::Extent2D& size, const vk::Offset2D& offset = vk::Offset2D( ))
        {
            return rect2D (size.width, size.height, offset.x, offset.y);
        }

        constexpr vk::AccessFlags accessFlagsForLayout (vk::ImageLayout layout)
        {
            switch (layout)
            {
                case vk::ImageLayout::ePreinitialized:
                    return vk::AccessFlagBits::eHostWrite;
                case vk::ImageLayout::eTransferDstOptimal:
                    return vk::AccessFlagBits::eTransferWrite;
                case vk::ImageLayout::eTransferSrcOptimal:
                    return vk::AccessFlagBits::eTransferRead;
                case vk::ImageLayout::eColorAttachmentOptimal:
                    return vk::AccessFlagBits::eColorAttachmentWrite;
                case vk::ImageLayout::eDepthStencilAttachmentOptimal:
                    return vk::AccessFlagBits::eDepthStencilAttachmentWrite;
                case vk::ImageLayout::eShaderReadOnlyOptimal:
                    return vk::AccessFlagBits::eShaderRead;
                default:
                    return vk::AccessFlags( );
            }
        }

        constexpr vk::PipelineStageFlags pipelineStageForLayout (vk::ImageLayout layout)
        {
            switch (layout)
            {
                case vk::ImageLayout::eTransferDstOptimal:
                case vk::ImageLayout::eTransferSrcOptimal:
                    return vk::PipelineStageFlagBits::eTransfer;

                case vk::ImageLayout::eColorAttachmentOptimal:
                    return vk::PipelineStageFlagBits::eColorAttachmentOutput;

                case vk::ImageLayout::eDepthStencilAttachmentOptimal:
                    return vk::PipelineStageFlagBits::eEarlyFragmentTests;

                case vk::ImageLayout::eShaderReadOnlyOptimal:
                    return vk::PipelineStageFlagBits::eFragmentShader;

                case vk::ImageLayout::ePreinitialized:
                    return vk::PipelineStageFlagBits::eHost;

                case vk::ImageLayout::eUndefined:
                    return vk::PipelineStageFlagBits::eTopOfPipe;

                default:
                    return vk::PipelineStageFlagBits::eBottomOfPipe;
            }
        }

        inline vk::ClearDepthStencilValue getClearValueDepth (float depth = 0, uint32_t stencil = 0)
        {
            const vk::ClearDepthStencilValue defaultClearDepth {depth, stencil};
            return defaultClearDepth;
        }

        enum class Attachment
        {
            COLOR,
            ALBEDO,
            ALPHA,
            NORMAL,
            POSITION,
            SPECULAR_EMISSION,
            LIGHT_SCATTERING,
            DEPTH
        };

        // Create an image memory barrier for changing the layout of
        // an image and put it into an active command buffer
        // See chapter 11.4 "vk::Image Layout" for details
        static void setImageLayout (const vk::CommandBuffer& cmdbuffer, const vk::Image& image,
                                    const vk::ImageLayout& oldImageLayout, const vk::ImageLayout& newImageLayout,
                                    const vk::ImageSubresourceRange& subresourceRange,
                                    vk::PipelineStageFlags srcStageMask = vk::PipelineStageFlagBits::eAllCommands,
                                    vk::PipelineStageFlags dstStageMask = vk::PipelineStageFlagBits::eAllCommands)
        {
            // Create an image barrier object
            vk::ImageMemoryBarrier imageMemoryBarrier;
            imageMemoryBarrier.oldLayout = oldImageLayout;
            imageMemoryBarrier.newLayout = newImageLayout;
            imageMemoryBarrier.image     = image;
            imageMemoryBarrier.subresourceRange =
                subresourceRange;  // warning C26478: Don't use std::move on constant variables. (es.56)
            imageMemoryBarrier.srcAccessMask = accessFlagsForLayout (oldImageLayout);
            imageMemoryBarrier.dstAccessMask = accessFlagsForLayout (newImageLayout);

            imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

            // Put barrier inside setup command buffer
            cmdbuffer.pipelineBarrier (srcStageMask, dstStageMask, vk::DependencyFlags( ), nullptr, nullptr,
                                       imageMemoryBarrier);
        }

    }  // namespace vkHelper
}  // namespace GraphicCore
#endif  // VK_HELPERS_HPP