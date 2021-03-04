// ******************************************************************************
//   File    : OpenGlBuffers.cpp
//   Project : i-Kan : Platform
//
//   Created by Ashish
// ******************************************************************************

#include <iKan/Platform/OpenGL/OpenGlBuffers.h>
#include <glad/glad.h>

namespace iKan {

    // ******************************************************************************
    // Open GL Vertex Buffer Constructor without data
    // ******************************************************************************
    OpenGlVertexBuffer::OpenGlVertexBuffer(uint32_t size)
    {
        IK_CORE_INFO("Open GL Vertex Buffer constructed without data of size {0}", size);

        glGenBuffers(1, &m_RendererId);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    // ******************************************************************************
    // Open GL Vertex Buffer Constructor with data
    // ******************************************************************************
    OpenGlVertexBuffer::OpenGlVertexBuffer(uint32_t size, float* data)
    {
        IK_CORE_INFO("Open GL Vertex Buffer constructed with data of size {0}", size);

        glGenBuffers(1, &m_RendererId);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }
    
    // ******************************************************************************
    // Open GL Vertex Buffer Destructor with data
    // ******************************************************************************
    OpenGlVertexBuffer::~OpenGlVertexBuffer()
    {
        IK_CORE_WARN("Open GL Vertex Buffer destroyed ...");

        glDeleteBuffers(1, &m_RendererId);
    }
    
    // ******************************************************************************
    // Bind Open GL Vertex Buffer
    // ******************************************************************************
    void OpenGlVertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
    }
    
    // ******************************************************************************
    // Unbind Open GL Vertex Buffer
    // ******************************************************************************
    void OpenGlVertexBuffer::Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    // ******************************************************************************
    // Set the Open GL Vertex Buffer data dynamycally
    // ******************************************************************************
    void OpenGlVertexBuffer::SetData(const void* data, uint32_t size)
    {
        IK_CORE_INFO("Setting the Open GL Vertex Buffer dynamically {0}", size);

        glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }
        
    // ******************************************************************************
    // Open GL Index Buffer Constructor
    // ******************************************************************************
    OpenGlIndexBuffer::OpenGlIndexBuffer(uint32_t count, uint32_t* data)
    : m_Count(count)
    {
        IK_CORE_INFO("Open GL Index Buffer constructed with count {0}", count);

        glGenBuffers(1, &m_RendererId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, data, GL_STATIC_DRAW);
    }
    
    // ******************************************************************************
    // Open GL Index Buffer Destructor
    // ******************************************************************************
    OpenGlIndexBuffer::~OpenGlIndexBuffer()
    {
        IK_CORE_WARN("Open GL Index Buffer destroyed");

        glDeleteBuffers(1, &m_RendererId);
    }
    
    // ******************************************************************************
    // Open GL Index Buffer Binding
    // ******************************************************************************
    void OpenGlIndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
    }
    
    // ******************************************************************************
    // Open GL Index Buffer Unbinding
    // ******************************************************************************
    void OpenGlIndexBuffer::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    
}
