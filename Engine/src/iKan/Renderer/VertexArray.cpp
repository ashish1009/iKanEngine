#include <iKan/Renderer/VertexArray.h>
#include <iKan/Renderer/RenderAPI.h>

#include <iKan/Core/Core.h>

#include <iKan/Platform/OpenGL/OpenGlVertexArray.h>

namespace iKan {
    
    Ref<VertexArray> VertexArray::Create()
    {
        switch (RenderAPI::GetAPI())
        {
            case API::None   : IK_CORE_ASSERT(false, "Render API not Supporting");
            case API::OpenGL : return Ref<OpenGlVertexArray>::Create(); break;
            default          : IK_CORE_ASSERT(false, "Invalid Render API ") break;
        }
        return nullptr;
    }
    
}
