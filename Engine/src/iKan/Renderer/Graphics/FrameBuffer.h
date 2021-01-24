#pragma once

#include <iKan/Core/Core.h>

namespace iKan {

    enum class FramebufferTextureFormat
    {
        None = 0,

        // Color
        RGBA8,

        // Depth/stencil
        DEPTH24STENCIL8,

        // ID
        R32I,

        // Defaults
        Depth = DEPTH24STENCIL8,
        ID = R32I

    };

    struct FramebufferTextureSpecification
    {
        FramebufferTextureSpecification() = default;
        FramebufferTextureSpecification(FramebufferTextureFormat format)
        : TextureFormat(format) {}

        FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
        // TODO: filtering/wrap
    };

    struct FramebufferAttachmentSpecification
    {
        FramebufferAttachmentSpecification() = default;
        FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
        : Attachments(attachments) {}

        std::vector<FramebufferTextureSpecification> Attachments;
    };

    struct FramebufferSpecification
    {
        uint32_t Width, Height;
        uint32_t Samples = 1;

        FramebufferAttachmentSpecification Attachments;

        bool SwapChainTarget = false;
    };
    
    class Framebuffer : public RefCounted
    {
    public:
        virtual ~Framebuffer() = default;
        
        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        
        virtual void Resize(uint32_t width, uint32_t height) = 0;
        
        virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;

        virtual const FramebufferSpecification& GetSpecification() const = 0;
        
        static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
    };
    
    
}
