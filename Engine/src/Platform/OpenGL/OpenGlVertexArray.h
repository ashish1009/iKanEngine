#pragma once

#include <iKan/Renderer/VertexArray.h>

namespace iKan {
    
    class OpenGlVertexArray : public VertexArray
    {
    public:
        OpenGlVertexArray();
        virtual ~OpenGlVertexArray();
        
        virtual void Bind() override;
        virtual void Unbind() override;
        
        virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer) override;
        void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;
        
    private:
        uint32_t m_RendererId;
        std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
    };
    
}
