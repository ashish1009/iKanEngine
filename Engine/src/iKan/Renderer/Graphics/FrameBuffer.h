// ******************************************************************************
//   File    : Framebuffer.h
//   Project : i-Kan : Renderer
//
//   Created by Ashish
// ******************************************************************************

#pragma once

namespace iKan {

    // ******************************************************************************
    // Enum for texture format. ONly interface, real conversion will be done
    // ******************************************************************************
    enum class FbTextureFormat
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

    // ******************************************************************************
    // Frame buffer Texture specification. Stores the texture format.
    // ******************************************************************************
    struct FbTextureSpecification
    {
        FbTextureSpecification() = default;
        FbTextureSpecification(FbTextureFormat format)
        : TextureFormat(format) {}

        FbTextureFormat TextureFormat = FbTextureFormat::None;
        // TODO: filtering/wrap
    };

    // ******************************************************************************
    // Frame buffer attachment specification. Stores all the attachment in a vector
    // ******************************************************************************
    struct FbAttachmentSpecification
    {
        FbAttachmentSpecification() = default;
        FbAttachmentSpecification(std::initializer_list<FbTextureSpecification> attachments)
        : Attachments(attachments) {}

        std::vector<FbTextureSpecification> Attachments;
    };

    // ******************************************************************************
    // Frame buffer specification. Store all the FB specificiaction
    // ******************************************************************************
    struct FramebufferSpecification
    {
        uint32_t Width, Height;
        uint32_t Samples = 1;

        FbAttachmentSpecification Attachments;

        bool SwapChainTarget = false;
    };
    
    // ******************************************************************************
    // Interface for Frame buffer class
    // ******************************************************************************
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
