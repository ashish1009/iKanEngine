// ******************************************************************************
//   File    : OpenGlShader.h
//   Project : i-Kan : Platform
//
//   Created by Ashish
// ******************************************************************************

#include <iKan/Platform/OpenGL/OpenGlShader.h>
#include <glad/glad.h>

namespace iKan {
    
    // ******************************************************************************
    // Get the Open GL Shader Type from string
    // ******************************************************************************
    static GLenum ShaderTypeFromString(const std::string& type)
    {
        if ("vertex" == type)   return GL_VERTEX_SHADER;
        if ("fragment" == type) return GL_FRAGMENT_SHADER;
        if ("geometry" == type) return GL_GEOMETRY_SHADER;

        IK_CORE_ASSERT(false, "Unknown shader type!");
        return 0;
    }
    
    // ******************************************************************************
    // Open GL Constructor
    // ******************************************************************************
    OpenGlShader::OpenGlShader(const std::string& vertexSrc, const std::string& fragmentSrc)
    {
        IK_CORE_INFO("Open GL Constructed from strings ");

        m_Source[GL_VERTEX_SHADER]   = vertexSrc;
        m_Source[GL_FRAGMENT_SHADER] = fragmentSrc;
        
        Compile();
    }
    
    // ******************************************************************************
    // Open GL Constructor
    // ******************************************************************************
    OpenGlShader::OpenGlShader(const std::string& path)
    {
        IK_CORE_INFO("Open GL Constructed from path : {0}", path.c_str());

        std::string source  = ReadFromFile(path);
        m_Source            = PreprocessFile(source);
        
        // Extract the name
        auto lastSlash = path.find_last_of("/\\");
        lastSlash      = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        
        auto lastDot = path.find_last_of('.');
        auto count   = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
        
        m_Name = path.substr(lastSlash, count);
        
        Compile();
    }
    
    // ******************************************************************************
    // Open GL Read from file
    // ******************************************************************************
    std::string OpenGlShader::ReadFromFile(const std::string& path)
    {
        IK_CORE_INFO("Reading Open GL Shdaer from file : {0}", path.c_str());

        std::string result;
        std::ifstream in(path, std::ios::in | std::ios::binary);
        if (in)
        {
            in.seekg(0, std::ios::end);
            size_t size = in.tellg();
            if (-1 != size)
            {
                result.resize(size);
                in.seekg(0, std::ios::beg);
                in.read(&result[0], size);
                in.close();
            }
            else
            {
                IK_CORE_ERROR("Could not read from file '{0}'", path);
            }
        }
        else
        {
            IK_CORE_ERROR("Could not open file '{0}'", path);
        }
        
        return result;
    }
    
    // ******************************************************************************
    // Process the Shader file
    // ******************************************************************************
    std::unordered_map<GLenum, std::string> OpenGlShader::PreprocessFile(const std::string& source)
    {
        IK_CORE_INFO("Process the Open GL Shader file : {0}", source.c_str());

        std::unordered_map<GLenum, std::string> shaderSource;
        
        const char* token = "#type";
        size_t pos        = source.find(token);
        while (pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos);
            IK_CORE_ASSERT((eol != std::string::npos), "Syntax error");

            size_t begin     = pos + strlen(token) + 1;
            std::string type = source.substr(begin, eol - begin);
            IK_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");
            
            size_t nextLine = source.find_first_of("\r\n", eol);
            pos             = source.find(token, nextLine);
            
            shaderSource[ShaderTypeFromString(type)] = source.substr(nextLine, pos - (nextLine == std::string::npos ? source.size() - 1 : nextLine));
        }
        
        return shaderSource;
    }
    
    // ******************************************************************************
    // Compile the Shader file
    // ******************************************************************************
    void OpenGlShader::Compile()
    {
        IK_CORE_INFO("Compiling Open GL Shader {0} ", m_Name.c_str());

        int32_t program = glCreateProgram();
        int32_t glShaderIDIndex = 0;
        std::array<uint32_t, 3> shaderId;

        for (auto& kv : m_Source)
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
            shaderId[glShaderIDIndex++] = shader;
        }
        
        // Create Shader program to activate and linke the shader
        m_RendererId = program;
        
        glLinkProgram(m_RendererId);
        
        // Error Handling
        // Note the different functions here: glGetProgram* instead of glGetShader
        GLint isLinked = 0;
        glGetProgramiv(m_RendererId, GL_LINK_STATUS, (int32_t*)&isLinked);
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
            {
                glDeleteShader(id);
            }
            
            IK_CORE_ERROR("{0}", infoLog.data());
            IK_CORE_ASSERT(false, "Shader link failure!");
        }
        for (auto id : shaderId)
            glDeleteShader(id);
    }
    
    // ******************************************************************************
    // Destructor for Open GL Shader file
    // ******************************************************************************
    OpenGlShader::~OpenGlShader()
    {
        IK_CORE_WARN("Destroying Open GL Shader : {0}", m_Name.c_str());

        glDeleteProgram(m_RendererId);
    }
    
    // ******************************************************************************
    // Bind Open GL Shader file
    // ******************************************************************************
    void OpenGlShader::Bind() const
    {
        glUseProgram(m_RendererId);
    }
    
    // ******************************************************************************
    // Unbind Open GL Shader file
    // ******************************************************************************
    void OpenGlShader::Unbind() const
    {
        glUseProgram(0);
    }
    
    // ******************************************************************************
    // Destructor for Open GL Shader file
    // ******************************************************************************
    void OpenGlShader::AddShaderReloadedCallback(const ShaderReloadedCallback& callback)
    {
        m_ShaderReloadedCallbacks.push_back(callback);
    }
    
    //-------------------------------- Uniforms --------------------------------
    void OpenGlShader::SetUniformInt1(const std::string& name, int value)
    {
        glUniform1i(GetUniformLocation(name), value);
    }
    
    void OpenGlShader::SetIntArray(const std::string& name, int* values, uint32_t count)
    {
        glUniform1iv(GetUniformLocation(name), count, values);
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
    
    // ******************************************************************************
    // get the uniform Location
    // ******************************************************************************
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
