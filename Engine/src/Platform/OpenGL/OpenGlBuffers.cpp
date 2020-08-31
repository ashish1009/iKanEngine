#include <Platform/OpenGL/OpenGlBuffers.h>

namespace iKan {

    // -------------------------------------- Vertex Buffer -------------------------------------
    OpenGlVertexBuffer::OpenGlVertexBuffer(uint32_t size, float* data)
    {
        glGenBuffers(1, &m_RendererId);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }
    
    OpenGlVertexBuffer::~OpenGlVertexBuffer()
    {
        glDeleteBuffers(1, &m_RendererId);
    }
    
    void OpenGlVertexBuffer::Bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
    }
    
    void OpenGlVertexBuffer::Unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
        
    // -------------------------------------- Index Buffer -------------------------------------
    OpenGlIndexBuffer::OpenGlIndexBuffer(uint32_t count, uint32_t* data)
    {
        glGenBuffers(1, &m_RendererId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, data, GL_STATIC_DRAW);
    }
    
    OpenGlIndexBuffer::~OpenGlIndexBuffer()
    {
        glDeleteBuffers(1, &m_RendererId);
    }
    
    void OpenGlIndexBuffer::Bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
    }
    
    void OpenGlIndexBuffer::Unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    
}
