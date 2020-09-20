#include <iKan/Renderer/VertexArray.h>
#include <iKan/Renderer/RenderAPI.h>

#include <iKan/Core/Core.h>

#include <iKan/Platform/OpenGL/OpenGlVertexArray.h>

namespace iKan {
    
    std::shared_ptr<VertexArray> VertexArray::Create()
    {
        switch (RenderAPI::GetAPI())
        {
            case API::None   : IK_CORE_ASSERT(false, "Render API not Supporting");
            case API::OpenGL : return std::make_shared<OpenGlVertexArray>(); break;
            default          : IK_CORE_ASSERT(false, "Invalid Render API ") break;
        }
        return nullptr;
    }
    
}
