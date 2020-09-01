#include <iKan/Renderer/RenderAPI.h>
#include <iKan/Core/Core.h>

#include <Platform/OpenGL/OpenGlRenderAPI.h>

namespace iKan {
    
    API RenderAPI::s_API = API::OpenGL;
    
    std::unique_ptr<RenderAPI> RenderAPI::Create()
    {
        switch (s_API)
        {
            case API::None   : IK_CORE_ASSERT(false, "Render API not Supporting");
            case API::OpenGL : return std::make_unique<OpenGlRenderAPI>(); break;
            default          : IK_CORE_ASSERT(false, "Invalid Render API ") break;
        }
        return nullptr;
    }
    
}