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
        
        // TODO: Find better place for this
        {
            s_Data.MeshShader->SetUniformFloat3("u_Light.Position", { 0.0f, 0.0f, -3.0f });
            s_Data.MeshShader->SetUniformFloat3("u_ViewPos", camera.ViewMatrix[3]);
            
            // light properties
            s_Data.MeshShader->SetUniformFloat3("u_Light.Ambient", { 0.2f, 0.2f, 0.2f });
            
            s_Data.MeshShader->SetUniformFloat3("u_Light.Diffuse", { 0.5f, 0.5f, 0.5f });
            
            s_Data.MeshShader->SetUniformFloat3("u_Light.Specular", { 1.0f, 1.0f, 1.0f });
            
            // material properties
            s_Data.MeshShader->SetUniformFloat1("u_Material.Shininess", 64.0f);
            s_Data.MeshShader->Unbind();
        }
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
