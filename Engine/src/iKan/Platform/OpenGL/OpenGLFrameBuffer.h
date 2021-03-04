// ******************************************************************************
//   File    : OpenGlFrameBuffer.h
//   Project : i-Kan : Platform
//
//   Created by Ashish
// ******************************************************************************

#pragma once

#include <iKan/Renderer/Graphics/FrameBuffer.h>

namespace iKan {
    
    // ******************************************************************************
    // Implementation of Open GL frame buffers
    // ******************************************************************************
    class OpenGLFrameBuffer : public Framebuffer
    {
    public:
        OpenGLFrameBuffer(const FramebufferSpecification& specs);
        virtual ~OpenGLFrameBuffer();
        
        virtual void Bind() override;
        virtual void Unbind() override;
        
        virtual const FramebufferSpecification& GetSpecification() const override { return m_Specifications; }
        virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override
        {
            IK_CORE_ASSERT((index < m_ColorAttachments.size()), "Invalid color attachment");
            return m_ColorAttachments[index];
        }

        virtual void Resize(uint32_t width, uint32_t height) override;

    private:
        void Invalidate();
        
    private:
        uint32_t                  m_RendererId = 0;
        FramebufferSpecification  m_Specifications;

        std::vector<FbTextureSpecification> m_ColorAttachmentSpecifications;
        FbTextureSpecification m_DepthAttachmentSpecification = FbTextureFormat::None;

        std::vector<uint32_t> m_ColorAttachments;
        uint32_t m_DepthAttachment = 0;
    };
    
}
