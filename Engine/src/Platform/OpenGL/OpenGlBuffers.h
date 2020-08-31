#pragma once

#include <iKan/Renderer/Buffers.h>

namespace iKan {
    
    class OpenGlVertexBuffer : public VertexBuffer
    {
    public:
        OpenGlVertexBuffer(uint32_t size, float* data);
        virtual ~OpenGlVertexBuffer();
        
        virtual void Bind() override;
        virtual void Unbind() override;
        
    private:
        uint32_t m_RendererId;
    };
    
    class OpenGlIndexBuffer : public IndexBuffer
    {
    public:
        OpenGlIndexBuffer(uint32_t count, uint32_t* data);
        virtual ~OpenGlIndexBuffer();
        
        virtual void Bind() override;
        virtual void Unbind() override;
        
    private:
        uint32_t m_RendererId;
    };

}
