#include <iKan/Renderer/GraphicsContext.h>
#include <iKan/Renderer/RendererAPI.h>

#include <Platform/OpenGL/OpenGlGraphicsContext.h>

#include <iKan/Core/Core.h>

namespace iKan {
    
    std::unique_ptr<GraphicsContext> GraphicsContext::CreateContext(GLFWwindow* window)
    {
        switch (RendererAPI::GetAPI())
        {
            case API::OpenGL : return std::make_unique<OpneGlGraphicsContext>(window); break;
            default          : IK_CORE_ASSERT(false, "Invalid Renderer API ") break;
        }
        return nullptr;
    }
    
}
