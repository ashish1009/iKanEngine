// ******************************************************************************
//   File    : Shader.cpp
//   Project : i-Kan : Renderer
//
//   Created by Ashish
// ******************************************************************************

#include <iKan/Renderer/Graphics/Shader.h>
#include <iKan/Renderer/API/RendererAPI.h>
#include <iKan/Platform/OpenGL/OpenGlShader.h>

namespace iKan {
    
    // ******************************************************************************
    // Creating instance for Shader depending on graphics API
    // ******************************************************************************
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
    
    // ******************************************************************************
    // Creating instance for Shader depending on graphics API
    // ******************************************************************************
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

    // ******************************************************************************
    // Loading shader into library
    // ******************************************************************************
    Ref<Shader> ShaderLibrary::Load(const std::string& path)
    {
        IK_CORE_INFO("Loadinh shader from path {0} into library", path.c_str());

        Ref<Shader> shader = Shader::Create(path);
        m_Shaders[shader->GetName()] = shader;
        return shader;
    }
    
    // ******************************************************************************
    // Loadinh shader into library
    // ******************************************************************************
    Ref<Shader> ShaderLibrary::Load(const std::string&name, const std::string& filapath)
    {
        IK_CORE_INFO("Loadinh shader {0} into library", name.c_str());

        Ref<Shader> shader = Shader::Create(filapath);
        m_Shaders[name] = shader;
        return shader;
    }

    // ******************************************************************************
    // Get the shader from library
    // ******************************************************************************
    Ref<Shader> ShaderLibrary::Get(const std::string& name)
    {
        IK_CORE_ASSERT(Exists(name), "Shader not found!");
        return m_Shaders[name];
    }
    
    // ******************************************************************************
    // ******************************************************************************
    bool ShaderLibrary::Exists(const std::string& name) const
    {
        return m_Shaders.find(name) != m_Shaders.end();
    }

}
