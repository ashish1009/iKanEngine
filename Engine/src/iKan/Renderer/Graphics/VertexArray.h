// ******************************************************************************
//   File    : VertexArray.h
//   Project : i-Kan : Renderer
//
//   Created by Ashish
// ******************************************************************************

#pragma once

#include <iKan/Renderer/Graphics/Buffers.h>

namespace iKan {
    
    // ******************************************************************************
    // Interface of Vertex Array Class
    // ******************************************************************************
    class VertexArray : public RefCounted
    {
    public:
        virtual ~VertexArray() = default;
        
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
        virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

        virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
        virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

        static Ref<VertexArray> Create();
    };
    
}
