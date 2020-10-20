#include <iKan/Renderer/API/RendererAPI.h>

#include <iKan/Core/Core.h>

#include <iKan/Platform/OpenGL/OpenGlRendererAPI.h>

namespace iKan {
    
    API RendererAPI::s_API = API::OpenGL;
    
    Scope<RendererAPI> RendererAPI::Create()
    {
        switch (s_API)
        {
            case API::None   : IK_CORE_ASSERT(false, "Render API not Supporting");
            case API::OpenGL : return CreateScope<OpenGlRendererAPI>(); break;
            default          : IK_CORE_ASSERT(false, "Invalid Render API ") break;
        }
        return nullptr;
    }
    
}
