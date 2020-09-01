#pragma once

#include <iKan/Renderer/Shader.h>

namespace iKan {
    
    class OpenGlShader : public Shader
    {
    public:
        OpenGlShader(const std::string& vertexSrc, const std::string& fragmantSrc);
        virtual ~OpenGlShader();
        
        virtual void Bind() const override;
        virtual void Unbind() const override;
        
        virtual void SetUniformInt1(const std::string& name, int value) override;
        
        virtual void SetUniformMat4(const std::string& name, const glm::mat4& value) override;
        virtual void SetUniformMat3(const std::string& name, const glm::mat3& value) override;
        
        virtual void SetUniformFloat1(const std::string& name, float value) override;
        virtual void SetUniformFloat2(const std::string& name, const glm::vec2& value) override;
        virtual void SetUniformFloat3(const std::string& name, const glm::vec3& value) override;
        virtual void SetUniformFloat4(const std::string& name, const glm::vec4& value) override;

    private:
        uint32_t m_RendererId;
    };
    
}
