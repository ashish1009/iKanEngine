#pragma once

#include <iKan/Renderer/Graphics/FrameBuffer.h>

namespace iKan {
    
    class OpenGLFrameBuffer : public Framebuffer
    {
    public:
        OpenGLFrameBuffer(const FramebufferSpecification& specs);
        virtual ~OpenGLFrameBuffer();
        
        virtual void Bind() override;
        virtual void Unbind() override;
        
        virtual const FramebufferSpecification& GetSpecification() const override { return m_Specifications; }
        virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }
        
        virtual void Resize(uint32_t width, uint32_t height) override;

    private:
        void Invalidate();
        
    private:
        uint32_t                    m_RendererId = 0;
        uint32_t                    m_ColorAttachment = 0, m_DepthAttachment = 0;
        FramebufferSpecification    m_Specifications;
        
    };
    
}
