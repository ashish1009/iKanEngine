#include <Platform/OpenGL/OpenGlShader.h>

#include <iKan/Core/Core.h>

namespace iKan {
    
    OpenGlShader::OpenGlShader(const std::string& vertexSrc, const std::string& fragmentSrc)
    {       
        // Creating Vertex Shader
        uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER); // DDD
        
        // Attch the shader source and then compile
        const char* vertecString = vertexSrc.c_str(); // DDD
        glShaderSource(vertexShader, 1, &vertecString, nullptr);
        glCompileShader(vertexShader);
        
        // Error Handling
        GLint isVertexCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isVertexCompiled);
        if (isVertexCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
            
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
            
            glDeleteShader(vertexShader);
            
            IK_CORE_ERROR("{0}", infoLog.data());
            IK_CORE_ASSERT(false, "Shader compilation failure!");
        }
        
        // Creating VertFragmentex Shader
        uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        
        // Attch the shader source and then compile
        const char* fragmentString = fragmentSrc.c_str();
        glShaderSource(fragmentShader, 1, &fragmentString, nullptr);
        glCompileShader(fragmentShader);
        
        // Error Handling
        GLint isFragmentCompiled = 0;
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isFragmentCompiled);
        if (isFragmentCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
            
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
            
            glDeleteShader(fragmentShader);
            
            IK_CORE_ERROR("{0}", infoLog.data());
            IK_CORE_ASSERT(false, "Shader compilation failure!");
        }
        
        // Create Shader program to activate and linke the shader
        m_RendererId = glCreateProgram();
        
        // Attach both shader and link them
        glAttachShader(m_RendererId, vertexShader);
        glAttachShader(m_RendererId, fragmentShader);
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
            
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            
            IK_CORE_ERROR("{0}", infoLog.data());
            IK_CORE_ASSERT(false, "Shader link failure!");
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
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
        int location = glGetUniformLocation(m_RendererId, name.c_str());
        
        if (-1 == location)
            IK_CORE_WARN("Warning: uniform '{0}' doesnt exist!", name);
        
        glUniform1i(location, value);
    }
    
    void OpenGlShader::SetUniformMat4(const std::string& name, const glm::mat4& value)
    {
        int location = glGetUniformLocation(m_RendererId, name.c_str());
        
        if (-1 == location)
            IK_CORE_WARN("Warning: uniform '{0}' doesnt exist", name);
        
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void OpenGlShader::SetUniformMat3(const std::string& name, const glm::mat3& value)
    {
        int location = glGetUniformLocation(m_RendererId, name.c_str());
        
        if (-1 == location)
            IK_CORE_WARN("Warning: uniform '{0}' doesnt exist", name);
        
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
    
    void OpenGlShader::SetUniformFloat1(const std::string& name, float value)
    {
        int location = glGetUniformLocation(m_RendererId, name.c_str());
        
        if (-1 == location)
            IK_CORE_WARN("Warning: uniform '{0}' doesnt exist", name);
        
        glUniform1f(location, value);
    }
    
    void OpenGlShader::SetUniformFloat2(const std::string& name, const glm::vec2& value)
    {
        int location = glGetUniformLocation(m_RendererId, name.c_str());
        
        if (-1 == location)
            IK_CORE_WARN("Warning: uniform '{0}' doesnt exist", name);
        
        glUniform2f(location, value.x, value.y);
    }
    
    void OpenGlShader::SetUniformFloat3(const std::string& name, const glm::vec3& value)
    {
        int location = glGetUniformLocation(m_RendererId, name.c_str());
        
        if (-1 == location)
            IK_CORE_WARN("Warning: uniform '{0}' doesnt exist", name);
        
        glUniform3f(location, value.x, value.y, value.z);
    }
    
    void OpenGlShader::SetUniformFloat4(const std::string& name, const glm::vec4& value)
    {
        int location = glGetUniformLocation(m_RendererId, name.c_str());
        
        if (-1 == location)
            IK_CORE_WARN("Warning: uniform '{0}' doesnt exist", name);
        
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }
    
}
