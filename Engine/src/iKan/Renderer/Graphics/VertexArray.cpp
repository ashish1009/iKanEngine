// ******************************************************************************
//   File    : VertexArray.cpp
//   Project : i-Kan : Renderer
//
//   Created by Ashish
// ******************************************************************************

#include <iKan/Renderer/Graphics/VertexArray.h>
#include <iKan/Renderer/API/RendererAPI.h>
#include <iKan/Platform/OpenGL/OpenGlVertexArray.h>

namespace iKan {
    
    // ******************************************************************************
    // creating instance of Verytex Array
    // ******************************************************************************
    Ref<VertexArray> VertexArray::Create()
    {
        switch (RendererAPI::GetAPI())
        {
            case API::None   : IK_CORE_ASSERT(false, "Render API not Supporting");
            case API::OpenGL : return Ref<OpenGlVertexArray>::Create(); break;
            default          : IK_CORE_ASSERT(false, "Invalid Render API ") break;
        }
        return nullptr;
    }
    
}
