#include <iKan/Platform/OpenGL/OpenGlBuffers.h>

#include <glad/glad.h>

namespace iKan {

    // -------------------------------------- Vertex Buffer -------------------------------------
    
    OpenGlVertexBuffer::OpenGlVertexBuffer(uint32_t size)
    {
        IK_PROFILE_FUNCTION();

        glGenBuffers(1, &m_RendererId);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    OpenGlVertexBuffer::OpenGlVertexBuffer(uint32_t size, float* data)
    {
        IK_PROFILE_FUNCTION();

        glGenBuffers(1, &m_RendererId);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }
    
    OpenGlVertexBuffer::~OpenGlVertexBuffer()
    {
        IK_PROFILE_FUNCTION();

        glDeleteBuffers(1, &m_RendererId);
    }
    
    void OpenGlVertexBuffer::Bind() const
    {
        IK_PROFILE_FUNCTION();

        glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
    }
    
    void OpenGlVertexBuffer::Unbind() const
    {
        IK_PROFILE_FUNCTION();

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    void OpenGlVertexBuffer::SetData(const void* data, uint32_t size)
    {
        IK_PROFILE_FUNCTION();

        glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }
        
    // -------------------------------------- Index Buffer -------------------------------------
    OpenGlIndexBuffer::OpenGlIndexBuffer(uint32_t count, uint32_t* data)
    : m_Count(count)
    {
        IK_PROFILE_FUNCTION();

        glGenBuffers(1, &m_RendererId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, data, GL_STATIC_DRAW);
    }
    
    OpenGlIndexBuffer::~OpenGlIndexBuffer()
    {
        IK_PROFILE_FUNCTION();

        glDeleteBuffers(1, &m_RendererId);
    }
    
    void OpenGlIndexBuffer::Bind() const
    {
        IK_PROFILE_FUNCTION();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
    }
    
    void OpenGlIndexBuffer::Unbind() const
    {
        IK_PROFILE_FUNCTION();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    
}
