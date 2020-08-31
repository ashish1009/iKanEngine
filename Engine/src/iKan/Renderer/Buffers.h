#pragma once

namespace iKan {
    
    // ------------------------- Vertex Buffer --------------------------------------------
    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;
        
        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        
        static std::shared_ptr<VertexBuffer> Create(uint32_t size, float* data);
    };
    
    // ------------------------- Index Buffer --------------------------------------------
    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;
        
        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        
        static std::shared_ptr<IndexBuffer> Create(uint32_t count, uint32_t* data);
    };
    
}
