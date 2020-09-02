#pragma once

namespace iKan {
    
    class Shader
    {
    public:
        virtual ~Shader() = default;
        
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        
        virtual void SetUniformInt1(const std::string& name, int value) = 0;
        
        virtual void SetUniformMat4(const std::string& name, const glm::mat4& value) = 0;
        virtual void SetUniformMat3(const std::string& name, const glm::mat3& value) = 0;
        
        virtual void SetUniformFloat1(const std::string& name, float value) = 0;
        virtual void SetUniformFloat2(const std::string& name, const glm::vec2& value) = 0;
        virtual void SetUniformFloat3(const std::string& name, const glm::vec3& value) = 0;
        virtual void SetUniformFloat4(const std::string& name, const glm::vec4& value) = 0;
        
        static std::shared_ptr<Shader> Create(const std::string& vertexSrc, const std::string& fragmentSrc);
        static std::shared_ptr<Shader> Create(const std::string& path);
    };
    
}
