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
        void Compile(const std::unordered_map<GLenum, std::string>& source);
        int32_t GetUniformLocation(const std::string& name);

    private:
        uint32_t m_RendererId;
        std::unordered_map<std::string, int32_t> m_LocationMap;
    };
    
}
