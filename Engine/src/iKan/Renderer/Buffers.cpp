#include <iKan/Renderer/Buffers.h>
#include <iKan/Renderer/RenderAPI.h>

#include <iKan/Core/Core.h>

#include <Platform/OpenGL/OpenGlBuffers.h>

namespace iKan {
    
    // ------------------------- Vertex Buffer --------------------------------------------
    std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size, float* data)
    {
        switch (RenderAPI::GetAPI())
        {
            case API::None   : IK_CORE_ASSERT(false, "Render API not Supporting");
            case API::OpenGL : return std::make_shared<OpenGlVertexBuffer>(size, data); break;
            default          : IK_CORE_ASSERT(false, "Invalid Render API ") break;
        }
        return nullptr;
    }
    
    std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
        switch (RenderAPI::GetAPI())
        {
            case API::None:    IK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case API::OpenGL:  return std::make_shared<OpenGlVertexBuffer>(size);
        }
        
        IK_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    // ------------------------- Index Buffer --------------------------------------------
    std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t size, uint32_t* data)
    {
        switch (RenderAPI::GetAPI())
        {
            case API::None   : IK_CORE_ASSERT(false, "Render API not Supporting");
            case API::OpenGL : return std::make_shared<OpenGlIndexBuffer>(size, data); break;
            default          : IK_CORE_ASSERT(false, "Invalid Render API ") break;
        }
        return nullptr;
    }

}
