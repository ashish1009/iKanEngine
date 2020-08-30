#include <iKan/Renderer/GraphicsContext.h>
#include <iKan/Renderer/RenderAPI.h>

#include <Platform/OpenGL/OpenGlGraphicsContext.h>

#include <iKan/Core/Core.h>

namespace iKan {
    
    std::unique_ptr<GraphicsContext> GraphicsContext::CreateContext(GLFWwindow* window)
    {
        switch (RenderAPI::GetAPI())
        {
            case API::None   : IK_CORE_ASSERT(false, "Render API not Supporting");
            case API::OpenGL : return std::make_unique<OpneGlGraphicsContext>(window); break;
            default          : IK_CORE_ASSERT(false, "Invalid Render API ") break;
        }
        return nullptr;
    }
    
}
