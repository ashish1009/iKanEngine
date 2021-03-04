// ******************************************************************************
//   File    : Renderer.cpp
//   Project : i-Kan : Renderer
//
//   Created by Ashish
// ******************************************************************************

#include <iKan/Renderer/API/Renderer.h>
#include <iKan/Renderer/API/Renderer2D.h>
#include <iKan/Renderer/API/SceneRenderer.h>
#include <iKan/Renderer/API/RendererStats.h>

namespace iKan {
    
    struct RendererData
    {
        Ref<ShaderLibrary> m_ShaderLibrary;
    };
    static RendererData s_Data;

    // ******************************************************************************
    // Initialise all renderer
    // ******************************************************************************
    void Renderer::Init()
    {
        RendererCommand::Init();
        Renderer2D::Init();
        SceneRenderer::Init();
        
        s_Data.m_ShaderLibrary = Ref<ShaderLibrary>::Create();
    }
    
    // ******************************************************************************
    // IClear color and buffer
    // ******************************************************************************
    void Renderer::Clear(glm::vec4 color)
    {
        RendererCommand::Clear();
        RendererCommand::SetClearColor(color);
    }
    
    // ******************************************************************************
    // Set the view port
    // ******************************************************************************
    void Renderer::SetViewport(float width, float height)
    {
        RendererCommand::SetViewPort(width, height);
    }
    
    // ******************************************************************************
    // Draw commanr
    // ******************************************************************************
    void Renderer::DrawIndexed(uint32_t count)
    {
        RendererStatistics::DrawCalls++;
        RendererCommand::DrawIndexed(count);
    }
    
    // ******************************************************************************
    // Draw command
    // ******************************************************************************
    void Renderer::DrawIndexed(const Ref<VertexArray> &vertexArray, uint32_t count)
    {
        RendererStatistics::DrawCalls++;
        RendererCommand::DrawIndexed(vertexArray, count);
    }
    
    // ******************************************************************************
    // Shutdown all renderer
    // ******************************************************************************
    void Renderer::Shutdown()
    {
        Renderer2D::Shutdown();
        SceneRenderer::Shutdown();
    }
    
    // ******************************************************************************
    //
    // ******************************************************************************
    Ref<ShaderLibrary> Renderer::GetShaderLibrary()
    {
        return s_Data.m_ShaderLibrary;
    }
    
}
