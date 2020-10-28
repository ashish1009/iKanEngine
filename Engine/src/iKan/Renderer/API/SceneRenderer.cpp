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
    
    void SceneRenderer::SetupLight(const SceneRendererLight &light)
    {
        s_Data.MeshShader->Bind();
        s_Data.MeshShader->SetUniformInt1("u_IsSceneLight", (light.Light != nullptr));

        if (light.Light)
        {
            s_Data.MeshShader->SetUniformFloat3("u_Light.Position", light.Position);
            s_Data.MeshShader->SetUniformFloat3("u_ViewPos", light.ViewPos);

            // light properties
            s_Data.MeshShader->SetUniformInt1("u_Light.IsAmbient", light.Light->IsAmbient);
            if (light.Light->IsAmbient)
                s_Data.MeshShader->SetUniformFloat3("u_Light.Ambient", light.Light->Ambient);
            
            s_Data.MeshShader->SetUniformInt1("u_Light.IsDiffuse", light.Light->IsDiffuse);
            if (light.Light->IsDiffuse)
                s_Data.MeshShader->SetUniformFloat3("u_Light.Diffuse", light.Light->Diffuse);
            
            s_Data.MeshShader->SetUniformInt1("u_Light.IsSpecular", light.Light->IsSpecular);
            if (light.Light->IsSpecular)
                s_Data.MeshShader->SetUniformFloat3("u_Light.Specular", light.Light->Specular);
            
            // material properties
            s_Data.MeshShader->SetUniformFloat1("u_Material.Shininess", 64.0f);
            s_Data.MeshShader->Unbind();
                        
        }
    }
    
    void SceneRenderer::BegineScene(const SceneRendererCamera& camera)
    {
        s_Data.MeshShader->Bind();
        s_Data.MeshShader->SetUniformMat4("u_ViewProjection", camera.Camera.GetProjection() * camera.ViewMatrix);
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
