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

    private:
        uint32_t m_RendererId;
    };
    
}
