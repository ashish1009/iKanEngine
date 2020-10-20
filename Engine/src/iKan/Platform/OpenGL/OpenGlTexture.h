#pragma once

#include <iKan/Renderer/Graphics/Texture.h>

#include <glad/glad.h>

namespace iKan {
    
    class OpenGlTexture : public Texture
    {
    public:
        OpenGlTexture(const std::string& path);
        OpenGlTexture(uint32_t width, uint32_t height, void* data, uint32_t size);
        
        virtual ~OpenGlTexture();
        
        virtual uint32_t GetWidth() const override { return m_Width;  }
        virtual uint32_t GetHeight() const override { return m_Height; }
        
        virtual void Bind(uint32_t slot = 0) const override;
        virtual void Unbind() const override;
        
        virtual uint32_t GetRendererID() const override { return m_RendererId; }
        
        virtual bool operator==(const Texture& other) const override
        {
            return m_RendererId == ((OpenGlTexture&)other).m_RendererId;
        }
        
    private:
        uint32_t m_RendererId;
        uint32_t m_Width, m_Height, m_Channel;
        GLenum m_InternalFormat = GL_RGB8, m_DataFormat = GL_RGB;
        
    };
    
    class OpenGlCubeMapTexture : public CubeMapTexture
    {
    public:
        OpenGlCubeMapTexture(std::vector<std::string> paths);
        OpenGlCubeMapTexture(const std::string& path);
        
        virtual ~OpenGlCubeMapTexture();
        virtual void Bind(uint32_t slot = 0) const override;
        
    private:
        uint32_t m_RendererId;
    };
    
}
