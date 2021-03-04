// ******************************************************************************
//   File    : GraphoicsContext.cpp
//   Project : i-Kan : Renderer
//
//   Created by Ashish
// ******************************************************************************

#include <iKan/Renderer/Graphics/GraphicsContext.h>
#include <iKan/Renderer/API/RendererAPI.h>
#include <iKan/Platform/OpenGL/OpenGlGraphicsContext.h>

namespace iKan {
    
    // ******************************************************************************
    // Creating Instance of Graphics context depending on the API
    // ******************************************************************************
    Scope<GraphicsContext> GraphicsContext::CreateContext(GLFWwindow* window)
    {
        switch (RendererAPI::GetAPI())
        {
            case API::None   : IK_CORE_ASSERT(false, "Render API not Supporting");
            case API::OpenGL : return CreateScope<OpneGlGraphicsContext>(window); break;
            default          : IK_CORE_ASSERT(false, "Invalid Render API ") break;
        }
        return nullptr;
    }
    
}
