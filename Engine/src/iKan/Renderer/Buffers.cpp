#include <iKan/Renderer/Buffers.h>
#include <iKan/Renderer/RenderAPI.h>

#include <iKan/Core/Core.h>

#include <iKan/Platform/OpenGL/OpenGlBuffers.h>

namespace iKan {
    
    // ------------------------- Vertex Buffer --------------------------------------------
    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, float* data)
    {
        switch (RenderAPI::GetAPI())
        {
            case API::None   : IK_CORE_ASSERT(false, "Render API not Supporting");
            case API::OpenGL : return Ref<OpenGlVertexBuffer>::Create(size, data); break;
            default          : IK_CORE_ASSERT(false, "Invalid Render API ") break;
        }
        return nullptr;
    }
    
    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
        switch (RenderAPI::GetAPI())
        {
            case API::None:    IK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case API::OpenGL:  return Ref<OpenGlVertexBuffer>::Create(size);
        }
        
        IK_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    // ------------------------- Index Buffer --------------------------------------------
    Ref<IndexBuffer> IndexBuffer::Create(uint32_t size, uint32_t* data)
    {
        switch (RenderAPI::GetAPI())
        {
            case API::None   : IK_CORE_ASSERT(false, "Render API not Supporting");
            case API::OpenGL : return Ref<OpenGlIndexBuffer>::Create(size, data); break;
            default          : IK_CORE_ASSERT(false, "Invalid Render API ") break;
        }
        return nullptr;
    }

}
