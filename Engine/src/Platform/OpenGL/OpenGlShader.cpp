#include <Platform/OpenGL/OpenGlShader.h>

#include <iKan/Core/Core.h>

namespace iKan {
    
    OpenGlShader::OpenGlShader(const std::string& vertexSrc, const std::string& fragmentSrc)
    {
        std::unordered_map<GLenum, std::string> source;
        source[GL_VERTEX_SHADER] = vertexSrc;
        source[GL_FRAGMENT_SHADER] = fragmentSrc;
        
        Compile(source);
    }
    
    void OpenGlShader::Compile(const std::unordered_map<GLenum, std::string>& source)
    {
        int program = glCreateProgram();
        std::array<uint32_t, 2> shaderId;
        int glShaderIDIndex = 0;

        for (auto& kv : source)
        {
            GLenum type = kv.first;
            std::string src = kv.second;
            
            uint32_t shader = glCreateShader(type);
            
            // Attch the shader source and then compile
            const char* string = src.c_str();
            glShaderSource(shader, 1, &string, nullptr);
            glCompileShader(shader);
            
            // Error Handling
            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
                
                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
                
                glDeleteShader(shader);
                
                IK_CORE_ERROR("{0}", infoLog.data());
                IK_CORE_ASSERT(false, "Shader compilation failure!");
            }
            // Attach both shader and link them
            glAttachShader(program, shader);
            shaderId[glShaderIDIndex] = shader;
        }
        
        // Create Shader program to activate and linke the shader
        m_RendererId = program;
        
        glLinkProgram(m_RendererId);
        
        // Error Handling
        // Note the different functions here: glGetProgram* instead of glGetShader
        GLint isLinked = 0;
        glGetProgramiv(m_RendererId, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(m_RendererId, GL_INFO_LOG_LENGTH, &maxLength);
            
            /* The maxLength includes the NULL character */
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(m_RendererId, maxLength, &maxLength, &infoLog[0]);
            
            /* We don't need the program anymore. */
            glDeleteProgram(m_RendererId);
            
            for (auto id : shaderId)
                glDeleteShader(id);
            
            IK_CORE_ERROR("{0}", infoLog.data());
            IK_CORE_ASSERT(false, "Shader link failure!");
        }
        for (auto id : shaderId)
            glDeleteShader(id);
    }
    
    OpenGlShader::~OpenGlShader()
    {
        glDeleteProgram(m_RendererId);
    }
    
    void OpenGlShader::Bind() const
    {
        glUseProgram(m_RendererId);
    }
    
    void OpenGlShader::Unbind() const
    {
        glUseProgram(0);
    }
    
    //-------------------------------- Uniforms --------------------------------
    void OpenGlShader::SetUniformInt1(const std::string& name, int value)
    {
        glUniform1i(GetUniformLocation(name), value);
    }
    
    void OpenGlShader::SetUniformMat4(const std::string& name, const glm::mat4& value)
    {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
    }

    void OpenGlShader::SetUniformMat3(const std::string& name, const glm::mat3& value)
    {
        glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
    }
    
    void OpenGlShader::SetUniformFloat1(const std::string& name, float value)
    {
        glUniform1f(GetUniformLocation(name), value);
    }
    
    void OpenGlShader::SetUniformFloat2(const std::string& name, const glm::vec2& value)
    {
        glUniform2f(GetUniformLocation(name), value.x, value.y);
    }
    
    void OpenGlShader::SetUniformFloat3(const std::string& name, const glm::vec3& value)
    {
        glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
    }
    
    void OpenGlShader::SetUniformFloat4(const std::string& name, const glm::vec4& value)
    {
        glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
    }
    
    int32_t OpenGlShader::GetUniformLocation(const std::string& name)
    {
        if (m_LocationMap.find(name) != m_LocationMap.end())
            return m_LocationMap[name];
        
        int32_t location = glGetUniformLocation(m_RendererId, name.c_str());
        if (-1 == location)
            IK_CORE_WARN("Warning: uniform '{0}' doesnt exist", name);

        m_LocationMap[name] = location;
        return location;
    }

    
}
