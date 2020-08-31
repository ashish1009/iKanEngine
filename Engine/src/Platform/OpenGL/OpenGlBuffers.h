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
        virtual void AddLayout(const BufferLayout& layout) override { m_Layout = layout; }
        virtual const BufferLayout& GetLayout() override { return m_Layout; }
        
    private:
        uint32_t     m_RendererId;
        BufferLayout m_Layout;
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
