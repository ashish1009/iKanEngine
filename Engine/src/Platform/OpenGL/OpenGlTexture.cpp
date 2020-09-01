#include <Platform/OpenGL/OpenGlTexture.h>

#include <iKan/Core/Core.h>

#include <stb_image.h>

namespace iKan {
 
    OpenGlTexture::OpenGlTexture(const std::string& path)
    {
        // Texture
        stbi_set_flip_vertically_on_load(1);
        
        int height, width, channel;
        
        stbi_uc* data = nullptr;
        data = stbi_load(path.c_str(), &width, &height, &channel, 0);
        IK_CORE_ASSERT(data, "Failed to load stbi Image");
        
        m_Width   = width;
        m_Height  = height;
        m_Channel = channel;
                
        if (4 == m_Channel)
        {
            m_InternalFormat = GL_RGBA8;
            m_DataFormat     = GL_RGBA;
        }
        else if (3 == channel)
        {
            m_InternalFormat = GL_RGB8;
            m_DataFormat     = GL_RGB;
        }
        IK_CORE_ASSERT((m_InternalFormat & m_DataFormat), "invalid Format ");
        
        glGenTextures(1, &m_RendererId);
        glBindTexture(GL_TEXTURE_2D, m_RendererId);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
        
        if (data)
            stbi_image_free(data);
    }
    
    OpenGlTexture::OpenGlTexture(uint32_t width, uint32_t height, void* data, uint32_t size)
    {
        m_Width  = width;
        m_Height = height;
        
        m_InternalFormat = GL_RGBA8;
        m_DataFormat = GL_RGBA;
        
        glGenTextures(1, &m_RendererId);
        glBindTexture(GL_TEXTURE_2D, m_RendererId);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        uint16_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
        IK_CORE_ASSERT((size == m_Width * m_Height * bpp), "Data must be entire texture");
        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
    }
    
    OpenGlTexture::~OpenGlTexture()
    {
        glDeleteTextures(1, &m_RendererId);
    }
    
    void OpenGlTexture::Bind(uint32_t slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_RendererId);
    }
    
    void OpenGlTexture::Unbind() const
    {
        
    }

}
    
