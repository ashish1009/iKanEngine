#include <iKan/Renderer/Shader.h>
#include <iKan/Renderer/RenderAPI.h>

#include <iKan/Core/Core.h>

#include <Platform/OpenGL/OpenGlShader.h>

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


}
