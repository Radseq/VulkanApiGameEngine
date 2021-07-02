#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include <FrameBufferAttachment.hpp>
#include <RenderPass.hpp>

#include "pch.h"

namespace GraphicCore
{
    class Framebuffer
    {
        const vk::Device &m_Device;
        vk::Framebuffer   m_Framebuffer {nullptr};

       public:
        Framebuffer (const vk::Device &Device);

        void Create (const FrameBufferAttachment &FBA, const RenderPass &RenderPass);

        Framebuffer (const Framebuffer &) = delete;

        Framebuffer (Framebuffer &&other);

        ~Framebuffer( );

        Framebuffer &operator= (const Framebuffer &) = delete;

        Framebuffer &operator= (Framebuffer &&) = delete;

        vk::Framebuffer GetVkFrameBuffer( ) const;
    };
}  // namespace GraphicCore

#endif  // FRAMEBUFFER_HPP
