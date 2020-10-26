#include <iKan/Renderer/API/SceneRenderer.h>

#include <iKan/Renderer/Graphics/Shader.h>

namespace iKan {
    
    struct SceneData
    {
        Ref<Shader> MeshShader;
    };
    static SceneData s_Data;
    
    void SceneRenderer::Init()
    {
        s_Data.MeshShader = Shader::Create("../../Engine/assets/shaders/MeshShader.glsl");
        
        // Creating array of Slots to store hem in shader
        int32_t samplers[16];
        for (uint32_t i = 0; i < 16; i++)
            samplers[i] = i;
        
        s_Data.MeshShader->Bind();
        s_Data.MeshShader->SetIntArray("u_Textures", samplers, 16);
        s_Data.MeshShader->Unbind();
    }
    
    void SceneRenderer::BegineScene(const SceneRendererCamera& camera)
    {
        s_Data.MeshShader->Bind();
        s_Data.MeshShader->SetUniformMat4("u_ViewProjection", camera.Camera.GetProjection() * camera.ViewMatrix);
        s_Data.MeshShader->Unbind();
    }
    
    void SceneRenderer::EndScene()
    {
        
    }
    
    void SceneRenderer::Draw(const Ref<Mesh>& mesh, const glm::mat4& transform)
    {
        s_Data.MeshShader->Bind();
        s_Data.MeshShader->SetUniformMat4("u_Transform", transform);
        s_Data.MeshShader->Unbind();
        
        mesh->Draw(*s_Data.MeshShader.Raw());
    }
    
    void SceneRenderer::Shutdown()
    {
        
    }
    
}
