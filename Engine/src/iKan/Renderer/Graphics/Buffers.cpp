// ******************************************************************************
//   File    : Buffers.cpp
//   Project : i-Kan : Renderer
//
//   Created by Ashish
// ******************************************************************************

#include <iKan/Renderer/Graphics/Buffers.h>
#include <iKan/Renderer/API/RendererAPI.h>
#include <iKan/Platform/OpenGL/OpenGlBuffers.h>

namespace iKan {
    
    // ******************************************************************************
    // Creating instance of Vertex Buffer
    // ******************************************************************************
    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, float* data)
    {
        switch (RendererAPI::GetAPI())
        {
            case API::None   : IK_CORE_ASSERT(false, "Render API not Supporting");
            case API::OpenGL : return Ref<OpenGlVertexBuffer>::Create(size, data); break;
            default          : IK_CORE_ASSERT(false, "Invalid Render API ") break;
        }
        return nullptr;
    }
    
    // ******************************************************************************
    // Creating instance of Vertex Buffer
    // ******************************************************************************
    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
        switch (RendererAPI::GetAPI())
        {
            case API::None:    IK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case API::OpenGL:  return Ref<OpenGlVertexBuffer>::Create(size);
        }
        
        IK_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    // ******************************************************************************
    // Creating instance of Index Buffer
    // ******************************************************************************
    Ref<IndexBuffer> IndexBuffer::Create(uint32_t size, uint32_t* data)
    {
        switch (RendererAPI::GetAPI())
        {
            case API::None   : IK_CORE_ASSERT(false, "Render API not Supporting");
            case API::OpenGL : return Ref<OpenGlIndexBuffer>::Create(size, data); break;
            default          : IK_CORE_ASSERT(false, "Invalid Render API ") break;
        }
        return nullptr;
    }

}
