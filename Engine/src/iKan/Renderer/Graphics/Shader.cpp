#include <iKan/Renderer/Graphics/Shader.h>

#include <iKan/Renderer/API/RendererAPI.h>

#include <iKan/Core/Core.h>

#include <iKan/Platform/OpenGL/OpenGlShader.h>

namespace iKan {
    
    Ref<Shader> Shader::Create(const std::string& vertexShader, const std::string& fragmentSrc)
    {
        switch (RendererAPI::GetAPI())
        {
            case API::None   : IK_CORE_ASSERT(false, "Render API not Supporting");
            case API::OpenGL : return Ref<OpenGlShader>::Create(vertexShader, fragmentSrc); break;
            default          : IK_CORE_ASSERT(false, "Invalid Render API ") break;
        }
        return nullptr;
    }
    
    Ref<Shader> Shader::Create(const std::string& path)
    {
        switch (RendererAPI::GetAPI())
        {
            case API::None   : IK_CORE_ASSERT(false, "Render API not Supporting");
            case API::OpenGL : return Ref<OpenGlShader>::Create(path); break;
            default          : IK_CORE_ASSERT(false, "Invalid Render API ") break;
        }
        return nullptr;
    }

    // ------------------------------------------ Shader Library -------------------------------------
    Ref<Shader> ShaderLibrary::Load(const std::string& path)
    {
        Ref<Shader> shader = Shader::Create(path);
        m_Shaders[shader->GetName()] = shader;
        return shader;
    }
    
    Ref<Shader> ShaderLibrary::Load(const std::string&name, const std::string& filapath)
    {
        Ref<Shader> shader = Shader::Create(filapath);
        m_Shaders[name] = shader;
        return shader;
    }

    Ref<Shader> ShaderLibrary::Get(const std::string& name)
    {
        IK_CORE_ASSERT(Exists(name), "Shader not found!");
        return m_Shaders[name];
    }
    
    bool ShaderLibrary::Exists(const std::string& name) const
    {
        return m_Shaders.find(name) != m_Shaders.end();
    }

}
