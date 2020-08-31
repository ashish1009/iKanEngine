#include <OpenGlVertexArray.h>

namespace iKan {
    
    OpenGlVertexArray::OpenGlVertexArray()
    {
        glGenVertexArrays(1, &m_RendererId);
        glBindVertexArray(m_RendererId);
    }
    
    OpenGlVertexArray::~OpenGlVertexArray()
    {
        glDeleteVertexArrays(1, &m_RendererId);
    }
    
    void OpenGlVertexArray::Bind()
    {
        glBindVertexArray(m_RendererId);
    }
    
    void OpenGlVertexArray::Unbind()
    {
        glBindVertexArray(0);

    }
    
}
