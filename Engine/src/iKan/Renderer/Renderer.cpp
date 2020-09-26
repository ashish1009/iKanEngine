#include <iKan/Renderer/Renderer.h>
#include <iKan/Renderer/Renderer2D.h>
#include <iKan/Renderer/SceneRenderer.h>
#include <iKan/Renderer/Renderer3D.h>

namespace iKan {
    
    struct RendererData
    {
        Ref<ShaderLibrary> m_ShaderLibrary;
    };
    static RendererData s_Data;

    void Renderer::Init()
    {
        RenderCommand::Init();
        Renderer2D::Init();
        SceneRenderer::Init();
        Renderer3D::Init();
        
        s_Data.m_ShaderLibrary = Ref<ShaderLibrary>::Create();
    }
    
    void Renderer::Clear(glm::vec4 color)
    {
        RenderCommand::Clear();
        RenderCommand::SetClearColor(color);
    }
    
    void Renderer::SetViewport(float width, float height)
    {
        RenderCommand::SetViewPort(width, height);
    }
    
    void Renderer::DrawIndexed(const Ref<VertexArray> &vertexArray, uint32_t count)
    {
        RenderCommand::DrawIndexed(vertexArray, count);
    }
    
    void Renderer::Shutdown()
    {
        Renderer2D::Shutdown();
        SceneRenderer::Shutdown();
    }
    
    Ref<ShaderLibrary> Renderer::GetShaderLibrary()
    {
        return s_Data.m_ShaderLibrary;
    }
    
}
