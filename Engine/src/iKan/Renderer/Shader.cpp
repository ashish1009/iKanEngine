#include <iKan/Renderer/Shader.h>
#include <iKan/Renderer/RenderAPI.h>

#include <iKan/Core/Core.h>

#include <iKan/Platform/OpenGL/OpenGlShader.h>

namespace iKan {
    
    std::shared_ptr<Shader> Shader::Create(const std::string& vertexShader, const std::string& fragmentSrc)
    {
        switch (RenderAPI::GetAPI())
        {
            case API::None   : IK_CORE_ASSERT(false, "Render API not Supporting");
            case API::OpenGL : return std::make_shared<OpenGlShader>(vertexShader, fragmentSrc); break;
            default          : IK_CORE_ASSERT(false, "Invalid Render API ") break;
        }
        return nullptr;
    }
    
    std::shared_ptr<Shader> Shader::Create(const std::string& path)
    {
        switch (RenderAPI::GetAPI())
        {
            case API::None   : IK_CORE_ASSERT(false, "Render API not Supporting");
            case API::OpenGL : return std::make_shared<OpenGlShader>(path); break;
            default          : IK_CORE_ASSERT(false, "Invalid Render API ") break;
        }
        return nullptr;
    }

    // ------------------------------------------ Shader Library -------------------------------------
    std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& path)
    {
        std::shared_ptr<Shader> shader = Shader::Create(path);
        m_Shaders[shader->GetName()] = shader;
        return shader;
    }
    
    std::shared_ptr<Shader> ShaderLibrary::Load(const std::string&name, const std::string& filapath)
    {
        std::shared_ptr<Shader> shader = Shader::Create(filapath);
        m_Shaders[name] = shader;
        return shader;
    }

    std::shared_ptr<Shader> ShaderLibrary::Get(const std::string& name)
    {
        IK_CORE_ASSERT(Exists(name), "Shader not found!");
        return m_Shaders[name];
    }
    
    bool ShaderLibrary::Exists(const std::string& name) const
    {
        return m_Shaders.find(name) != m_Shaders.end();
    }

}
