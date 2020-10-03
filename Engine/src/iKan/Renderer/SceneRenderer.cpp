#include <iKan/Renderer/SceneRenderer.h>
#include <iKan/Renderer/Renderer.h>
#include <iKan/Renderer/Shader.h>
#include <iKan/Renderer/VertexArray.h>
#include <iKan/Renderer/Buffers.h>
#include <iKan/Renderer/Texture.h>
#include <iKan/Renderer/RenderStats.h>
#include <iKan/Renderer/Mesh.h>
#include <iKan/Core/GlmMath.h>

namespace iKan {
    
    struct SceneRendererData
    {
        static const uint32_t MaxTextureSlots = 16;
        
        SceneRendererCamera SceneCamera;
        Light ActiveLight;
        
        Ref<Texture> WhiteTexture;
        Ref<Shader>  Shader;
    };
    static SceneRendererData s_Data;
    
    void SceneRenderer::Init()
    {
        // Creating array of Slots to store hem in shader
        int32_t samplers[s_Data.MaxTextureSlots];
        for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
            samplers[i] = i;
        
        // Creating Shader and storing all the slots
        s_Data.Shader = Shader::Create("../../Editor/assets/shaders/CommonShader.glsl");
        s_Data.Shader->Bind();
        s_Data.Shader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);
        
        // Creating whote texture for cololful quads witout any texture or sprite
        uint32_t whiteTextureData = 0xffffffff;
        s_Data.WhiteTexture       = Texture::Create(1, 1, &whiteTextureData, sizeof(uint32_t));
    }
    
    void SceneRenderer::SetShaader(const std::string &path)
    {
    }
    
    void SceneRenderer::Shutdown()
    {
    }
    
    void SceneRenderer::BeginScene(const Ref<Scene>& scene, const SceneRendererCamera& camera)
    {
        // Copy light property
        s_Data.ActiveLight = scene->GetLight();
        
        // Upload Camera View Projection Matris to shader
        glm::mat4 viewProj = camera.Camera.GetProjection() * camera.ViewMatrix;
                
        s_Data.Shader->Bind();
        s_Data.Shader->SetUniformMat4("u_ViewProjection", viewProj);
        
        // Binding White Texture
        s_Data.WhiteTexture->Bind();
        
        // Upload uniforms
        auto& lightProp = s_Data.ActiveLight;
        auto shader = s_Data.Shader;
        shader->Bind();
        shader->SetUniformFloat3("u_Light.Position", lightProp.Position);
        shader->SetUniformFloat3("u_ViewPos", s_Data.SceneCamera.ViewMatrix[3]);
        
        // light properties
        shader->SetUniformInt1("u_LightFlag.IsAmbient", lightProp.LightFlag.IsAmbient);
        if (lightProp.LightFlag.IsAmbient)
            shader->SetUniformFloat3("u_Light.Ambient", lightProp.Ambient);
        
        shader->SetUniformInt1("u_LightFlag.IsDiffuse", lightProp.LightFlag.IsDiffuse);
        if (lightProp.LightFlag.IsAmbient)
            shader->SetUniformFloat3("u_Light.Diffuse", lightProp.Diffuse);
        
        shader->SetUniformInt1("u_LightFlag.IsSpecular", lightProp.LightFlag.IsSpecular);
        if (lightProp.LightFlag.IsAmbient)
            shader->SetUniformFloat3("u_Light.Specular", lightProp.Specular);
        
        // material properties
        shader->SetUniformFloat1("u_Material.Shininess", 64.0f);
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
    
    void SceneRenderer::DrawMesh(const Ref<Mesh>& mesh, const glm::mat4& transform)
    {
        auto [translation, rotationQuat, scale] = GlmMath::GetTransformDecomposition(transform);
        glm::vec3 rotation = glm::degrees(glm::eulerAngles(rotationQuat));
        
        glm::mat4 modTransform = glm::translate(glm::mat4(1.0f), s_Data.ActiveLight.Position) *
                                 glm::toMat4(glm::quat(glm::radians(rotation))) *
                                 glm::scale(glm::mat4(1.0f), scale);
        
        s_Data.Shader->Bind();
        s_Data.Shader->SetUniformMat4("u_Transform", modTransform);

        mesh->Draw(*s_Data.Shader.Raw());
    }
    
}
