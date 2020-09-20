#pragma once

namespace iKan {
    
    class Shader : public RefCounted
    {
    public:
        virtual ~Shader() = default;
        
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        
        virtual const std::string& GetName() = 0;
        
        virtual void SetUniformInt1(const std::string& name, int value) = 0;
        virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;
        
        virtual void SetUniformMat4(const std::string& name, const glm::mat4& value) = 0;
        virtual void SetUniformMat3(const std::string& name, const glm::mat3& value) = 0;
        
        virtual void SetUniformFloat1(const std::string& name, float value) = 0;
        virtual void SetUniformFloat2(const std::string& name, const glm::vec2& value) = 0;
        virtual void SetUniformFloat3(const std::string& name, const glm::vec3& value) = 0;
        virtual void SetUniformFloat4(const std::string& name, const glm::vec4& value) = 0;
        
        static Ref<Shader> Create(const std::string& vertexSrc, const std::string& fragmentSrc);
        static Ref<Shader> Create(const std::string& path);
    };
    
    class ShaderLibrary
    {
    public:
        ShaderLibrary() = default;
        ~ShaderLibrary() = default;
        
        Ref<Shader> Get(const std::string& name);
        
        Ref<Shader> Load(const std::string& path);
        Ref<Shader> Load(const std::string& name, const std::string& filepath);
        
    private:
        bool Exists(const std::string& name) const;
        
    private:
        std::unordered_map<std::string, Ref<Shader>> m_Shaders;
    };
    
}
