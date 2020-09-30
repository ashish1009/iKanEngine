#include <iKan/Renderer/SceneRenderer.h>
#include <iKan/Renderer/Renderer.h>
#include <iKan/Renderer/Shader.h>
#include <iKan/Renderer/VertexArray.h>
#include <iKan/Renderer/Buffers.h>
#include <iKan/Renderer/Texture.h>
#include <iKan/Renderer/RenderStats.h>
#include <iKan/Renderer/Mesh.h>

namespace iKan {
    
    static glm::vec3 s_LightPos = { 0.0f, 0.0f, 0.0f };
    
    struct SceneRendererData
    {
        static const uint32_t MaxTextureSlots = 16;
        
        SceneRendererCamera SceneCamera;
        
        Ref<Mesh>   Mesh;
        Ref<Shader> Shader;
    };
    static SceneRendererData s_Data;
    
    void SceneRenderer::Init()
    {
        s_Data.Mesh = Ref<Mesh>::Create("../../Editor/assets/resources/objects/backpack/backpack.obj");
        
        // Creating array of Slots to store hem in shader
        int32_t samplers[s_Data.MaxTextureSlots];
        for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
            samplers[i] = i;
        
        // Creating Shader and storing all the slots
        s_Data.Shader = Shader::Create("../../Editor/assets/shaders/CommonShader.glsl");
        s_Data.Shader->Bind();
        s_Data.Shader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

    }
    
    void SceneRenderer::SetShaader(const std::string &path)
    {
    }
    
    void SceneRenderer::Shutdown()
    {
    }
    
    void SceneRenderer::BeginScene(const SceneRendererCamera& camera)
    {
        // Upload Camera View Projection Matris to shader
        glm::mat4 viewProj = camera.Camera.GetProjection() * camera.ViewMatrix;
        
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
        s_Data.Shader->Bind();
        s_Data.Shader->SetUniformFloat3("u_Light.Position", s_LightPos);
        s_Data.Shader->SetUniformFloat3("u_ViewPos", s_Data.SceneCamera.ViewMatrix[3]);
        
        // light properties
        s_Data.Shader->SetUniformFloat3("u_Light.Ambient", { 0.2f, 0.2f, 0.2f });
        s_Data.Shader->SetUniformFloat3("u_Light.Diffuse", { 0.5f, 0.5f, 0.5f });
        s_Data.Shader->SetUniformFloat3("u_Light.Specular", { 1.0f, 1.0f, 1.0f });
        
        // material properties
        s_Data.Shader->SetUniformFloat1("u_Material.Shininess", 64.0f);
        
        s_Data.Mesh->Draw(*s_Data.Shader.Raw());
    }
    
}
