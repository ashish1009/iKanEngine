// ******************************************************************************
//   File    : OpenGlBuffers.h
//   Project : i-Kan : Platform
//
//   Created by Ashish
// ******************************************************************************

#pragma once

#include <iKan/Renderer/Graphics/Buffers.h>

namespace iKan {
    
    // ******************************************************************************
    // Implementation of Open GL Vertex Buffer
    // ******************************************************************************
    class OpenGlVertexBuffer : public VertexBuffer
    {
    public:
        OpenGlVertexBuffer(uint32_t size);
        OpenGlVertexBuffer(uint32_t size, float* data);
        virtual ~OpenGlVertexBuffer();
        
        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void SetData(const void* data, uint32_t size) override;

        virtual void AddLayout(const BufferLayout& layout) override { m_Layout = layout; }
        virtual const BufferLayout& GetLayout() const override { return m_Layout; }
        
    private:
        uint32_t     m_RendererId;
        BufferLayout m_Layout;
    };

    // ******************************************************************************
    // Implementation of Open GL Index Buffer
    // ******************************************************************************
    class OpenGlIndexBuffer : public IndexBuffer
    {
    public:
        OpenGlIndexBuffer(uint32_t count, uint32_t* data);
        virtual ~OpenGlIndexBuffer();
        
        virtual void Bind() const override;
        virtual void Unbind() const override;
        
        virtual uint32_t GetCount() const override { return m_Count; }
        
    private:
        uint32_t m_RendererId;
        uint32_t m_Count;
    };

}
