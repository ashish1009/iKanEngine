#include <iKan/Renderer/SceneRenderer.h>
#include <iKan/Renderer/Renderer.h>
#include <iKan/Renderer/Shader.h>
#include <iKan/Renderer/VertexArray.h>
#include <iKan/Renderer/Buffers.h>
#include <iKan/Renderer/Texture.h>
#include <iKan/Renderer/RenderStats.h>
#include <iKan/Renderer/Model.h>

namespace iKan {
    
    struct SceneRendererData
    {
        Scope<Model> Model;
        Ref<Shader>  Shader;
    };
    static SceneRendererData s_Data;
    
    void SceneRenderer::Init()
    {
        s_Data.Model = CreateScope<Model>("../../Editor/assets/resources/objects/pokemon/Pokemon.obj");
        s_Data.Shader = Shader::Create("../../Editor/assets/shaders/CommonShader.glsl");
    }
    
    void SceneRenderer::SetShaader(const std::string &path)
    {
    }
    
    void SceneRenderer::Shutdown()
    {
    }
    
    void SceneRenderer::BeginScene(const Camera& camera, const glm::mat4& transform)
    {
        // Upload Camera View Projection Matris to shader
        glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);
        
        s_Data.Shader->Bind();
        s_Data.Shader->SetUniformMat4("u_ViewProjection", viewProj);
    }
    
    void SceneRenderer::EndScene()
    {
    }
    
    void SceneRenderer::Flush()
    {
    }
    
    void SceneRenderer::FlushAndReset()
    {
    }
    
    void SceneRenderer::Draw()
    {
        s_Data.Model->Draw(*s_Data.Shader.Raw());
    }
    
}
