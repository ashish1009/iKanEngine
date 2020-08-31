#pragma once

#include <iKan/Renderer/Buffers.h>

namespace iKan {
    
    class VertexArray
    {
    public:
        virtual ~VertexArray() = default;
        
        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer) = 0;
        virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

        static std::shared_ptr<VertexArray> Create();
    };
    
}
