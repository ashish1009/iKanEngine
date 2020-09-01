#pragma once

#include <iKan/Renderer/Texture.h>

namespace iKan {
    
    class OpenGlTexture : public Texture
    {
    public:
        OpenGlTexture(const std::string& path);
        OpenGlTexture(uint32_t width, uint32_t height, void* data, uint32_t size);
        
        virtual ~OpenGlTexture();
        
        virtual void Bind(uint32_t slot = 0) const override;
        virtual void Unbind() const override;
        
    private:
        uint32_t m_RendererId;
        uint32_t m_Width, m_Height, m_Channel;
        GLenum m_InternalFormat = GL_RGB8, m_DataFormat = GL_RGB;
        
    };
    
}
