#include <OpenGlVertexArray.h>

namespace iKan {
    
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::NoType:   return 0;
            case ShaderDataType::Float:    return GL_FLOAT;
            case ShaderDataType::Float2:   return GL_FLOAT;
            case ShaderDataType::Float3:   return GL_FLOAT;
            case ShaderDataType::Float4:   return GL_FLOAT;
            case ShaderDataType::Mat3:     return GL_FLOAT;
            case ShaderDataType::Mat4:     return GL_FLOAT;
            case ShaderDataType::Int:      return GL_INT;
            case ShaderDataType::Int2:     return GL_INT;
            case ShaderDataType::Int3:     return GL_INT;
            case ShaderDataType::Int4:     return GL_INT;
            case ShaderDataType::Bool:     return GL_BOOL;
        }
        
        IK_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }
    
    OpenGlVertexArray::OpenGlVertexArray()
    {
        glGenVertexArrays(1, &m_RendererId);
        glBindVertexArray(m_RendererId);
    }
    
    OpenGlVertexArray::~OpenGlVertexArray()
    {
        glDeleteVertexArrays(1, &m_RendererId);
    }
    
    void OpenGlVertexArray::Bind() const
    {
        glBindVertexArray(m_RendererId);
    }
    
    void OpenGlVertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }
    
    void OpenGlVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
    {
        glBindVertexArray(m_RendererId);
        m_VertexBuffers.push_back(vertexBuffer);
        
        uint32_t index = 0;
        const auto& layout = vertexBuffer->GetLayout();
        for (const auto& element : layout.GetElements())
        {
            switch (element.Type)
            {
                case ShaderDataType::Float:
                case ShaderDataType::Float2:
                case ShaderDataType::Float3:
                case ShaderDataType::Float4:
                case ShaderDataType::Int:
                case ShaderDataType::Int2:
                case ShaderDataType::Int3:
                case ShaderDataType::Int4:
                case ShaderDataType::Bool:
                {
                    glEnableVertexAttribArray(index);
                    glVertexAttribPointer(index,
                                          element.Count,
                                          ShaderDataTypeToOpenGLBaseType(element.Type),
                                          element.Normalized ? GL_TRUE : GL_FALSE,
                                          layout.GetStride(),
                                          (const void*)element.Offset);
                    index++;
                    break;
                }
                case ShaderDataType::Mat3:
                case ShaderDataType::Mat4:
                {
                    uint8_t count = element.Count;
                    for (uint8_t i = 0; i < count; i++)
                    {
                        glEnableVertexAttribArray(index);
                        glVertexAttribPointer(index,
                                              count,
                                              ShaderDataTypeToOpenGLBaseType(element.Type),
                                              element.Normalized ? GL_TRUE : GL_FALSE,
                                              layout.GetStride(),
                                              (const void*)(sizeof(float) * count * i));
                        glVertexAttribDivisor(index, 1);
                        index++;
                    }
                    break;
                }
                default:
                    IK_CORE_ASSERT(false, "Unknown ShaderDataType!");
            }
        }
    }
    
    void OpenGlVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
    {
        glBindVertexArray(m_RendererId);
        indexBuffer->Bind();
        
        m_IndexBuffer = indexBuffer;
    }

    
}
