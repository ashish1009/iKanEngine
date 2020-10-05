#include <iKan/Renderer/SceneRenderer.h>
#include <iKan/Renderer/Renderer.h>
#include <iKan/Renderer/Shader.h>
#include <iKan/Renderer/VertexArray.h>
#include <iKan/Renderer/Buffers.h>
#include <iKan/Renderer/Texture.h>
#include <iKan/Renderer/RenderStats.h>
#include <iKan/Renderer/Mesh.h>

namespace iKan {
    
    struct SceneRendererData
    {
        static const uint32_t MaxTextureSlots = 16;

        struct CubeMapData
        {
            Ref<VertexArray>    VertexArray;
            Ref<VertexBuffer>   VertexBuffer;
            Ref<CubeMapTexture> Texture;
        };
        CubeMapData CubeMapData;
        
        SceneRendererCamera SceneCamera;
        Light ActiveLight;
        
        Ref<Texture> WhiteTexture;
        
        ShaderLibrary Shaders;
    };
    static SceneRendererData s_Data;
    
    void SceneRenderer::InitMeshData()
    {
        // Creating array of Slots to store hem in shader
        int32_t samplers[s_Data.MaxTextureSlots];
        for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
            samplers[i] = i;
        
        //TODO: Load Shader from outside or move the files inside Engine
        // Creating Shader and storing all the slots
        auto lightSourceShader = s_Data.Shaders.Load("../../Editor/assets/shaders/LightSourceShader.glsl");
        lightSourceShader->Bind();
        lightSourceShader->SetUniformInt1("u_Texture", 0);
        lightSourceShader->Unbind();
        
        auto adsShader = s_Data.Shaders.Load("../../Editor/assets/shaders/ADS_Shader.glsl");
        adsShader->Bind();
        adsShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);
        adsShader->Unbind();
        
        // Creating whote texture for cololful quads witout any texture or sprite
        uint32_t whiteTextureData = 0xffffffff;
        s_Data.WhiteTexture       = Texture::Create(1, 1, &whiteTextureData, sizeof(uint32_t));
    }
    
    void SceneRenderer::InitCubeMapData()
    {
        float skyboxVertices[] =
        {
            -5.0f,  5.0f, -5.0f,
            -5.0f, -5.0f, -5.0f,
             5.0f, -5.0f, -5.0f,
             5.0f, -5.0f, -5.0f,
             5.0f,  5.0f, -5.0f,
            -5.0f,  5.0f, -5.0f,
            
            -5.0f, -5.0f,  5.0f,
            -5.0f, -5.0f, -5.0f,
            -5.0f,  5.0f, -5.0f,
            -5.0f,  5.0f, -5.0f,
            -5.0f,  5.0f,  5.0f,
            -5.0f, -5.0f,  5.0f,
            
             5.0f, -5.0f, -5.0f,
             5.0f, -5.0f,  5.0f,
             5.0f,  5.0f,  5.0f,
             5.0f,  5.0f,  5.0f,
             5.0f,  5.0f, -5.0f,
             5.0f, -5.0f, -5.0f,
            
            -5.0f, -5.0f,  5.0f,
            -5.0f,  5.0f,  5.0f,
             5.0f,  5.0f,  5.0f,
             5.0f,  5.0f,  5.0f,
             5.0f, -5.0f,  5.0f,
            -5.0f, -5.0f,  5.0f,
            
            -5.0f,  5.0f, -5.0f,
             5.0f,  5.0f, -5.0f,
             5.0f,  5.0f,  5.0f,
             5.0f,  5.0f,  5.0f,
            -5.0f,  5.0f,  5.0f,
            -5.0f,  5.0f, -5.0f,
            
            -5.0f, -5.0f, -5.0f,
            -5.0f, -5.0f,  5.0f,
             5.0f, -5.0f, -5.0f,
             5.0f, -5.0f, -5.0f,
             -5.0f, -5.0f,  5.0f,
            5.0f, -5.0f,  5.0f
        };
        
        /* Creating Vertex Array */
        s_Data.CubeMapData.VertexArray = VertexArray::Create();
        
        /* Creating Vertex Buffer and adding Layout */
        s_Data.CubeMapData.VertexBuffer = VertexBuffer::Create(sizeof(skyboxVertices), skyboxVertices);
        s_Data.CubeMapData.VertexBuffer->AddLayout({
            { ShaderDataType::Float3, "a_Position" }
        });
        s_Data.CubeMapData.VertexArray->AddVertexBuffer(s_Data.CubeMapData.VertexBuffer);
        
        /* Shader Bind and texture set*/
        auto cubeMapShader = s_Data.Shaders.Load("../../Editor/assets/shaders/CubeMap.glsl");
        cubeMapShader->Bind();
        cubeMapShader->SetUniformInt1("u_Skybox", 0);
    }
    
    void SceneRenderer::Init()
    {
        InitMeshData();
        InitCubeMapData();
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
        auto& lightProp = s_Data.ActiveLight;
        
        // Upload Camera View Projection Matris to shader
        glm::mat4 viewProj = camera.Camera.GetProjection() * camera.ViewMatrix;
        
        // Mesh Shader Camera Bind
        for (auto kv : s_Data.Shaders.GetShaders())
        {
            auto shader = kv.second;
            shader->Bind();
            shader->SetUniformMat4("u_ViewProjection", viewProj);
            shader->Unbind();
        }

        // Cube Map shader bind the camera
        auto cubeMapShader = s_Data.Shaders.Get("CubeMap");
        cubeMapShader->Bind();
        cubeMapShader->SetUniformMat4("u_ProjectionView", viewProj);

        
        // TODO: Try to move these somewhere else
        auto ads_Shader = s_Data.Shaders.Get("ADS_Shader");
        // Upload uniforms
        ads_Shader->Bind();
        ads_Shader->SetUniformFloat3("u_Light.Position", lightProp.Position);
        ads_Shader->SetUniformFloat3("u_ViewPos", s_Data.SceneCamera.ViewMatrix[3]);
        
        // light properties
        ads_Shader->SetUniformInt1("u_LightFlag.IsAmbient", lightProp.LightFlag.IsAmbient);
        if (lightProp.LightFlag.IsAmbient)
            ads_Shader->SetUniformFloat3("u_Light.Ambient", lightProp.Ambient);
        
        ads_Shader->SetUniformInt1("u_LightFlag.IsDiffuse", lightProp.LightFlag.IsDiffuse);
        if (lightProp.LightFlag.IsAmbient)
            ads_Shader->SetUniformFloat3("u_Light.Diffuse", lightProp.Diffuse);
        
        ads_Shader->SetUniformInt1("u_LightFlag.IsSpecular", lightProp.LightFlag.IsSpecular);
        if (lightProp.LightFlag.IsAmbient)
            ads_Shader->SetUniformFloat3("u_Light.Specular", lightProp.Specular);
        
        ads_Shader->SetUniformInt1("u_LightFlag.IsAttenuation", lightProp.LightFlag.IsAttenuation);
        if (lightProp.LightFlag.IsAttenuation)
        {
            ads_Shader->SetUniformFloat1("u_Light.Constant", lightProp.Constant);
            ads_Shader->SetUniformFloat1("u_Light.Linear", lightProp.Linear);
            ads_Shader->SetUniformFloat1("u_Light.Quadratic", lightProp.Quadratic);
        }
        
        ads_Shader->SetUniformInt1("u_LightFlag.IsSpotLight", lightProp.LightFlag.IsSpotLight);
        if (lightProp.LightFlag.IsSpotLight)
        {
            ads_Shader->SetUniformFloat3("u_Light.Direction", camera.Front);
            ads_Shader->SetUniformFloat1("u_Light.CutOff", glm::cos(glm::radians(lightProp.CutOff)));
        }
        
        // material properties
        ads_Shader->SetUniformFloat1("u_Material.Shininess", 64.0f);
        ads_Shader->Unbind();
        
        // Binding White Texture
        s_Data.WhiteTexture->Bind();
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
    
    void SceneRenderer::SetCubeMapTexture(const std::vector<std::string>& paths)
    {
        s_Data.CubeMapData.Texture = CubeMapTexture::Create(paths);
        RendererStatistics::TextureCount += 6;
    }
    
    // TODO: fix these flags , shouldnt be here
    void SceneRenderer::DrawMesh(const Ref<Mesh>& mesh, const glm::mat4& transform, bool isADS, bool isLightSOurce)
    {
        for (auto kv : s_Data.Shaders.GetShaders())
        {
            auto shader = kv.second;
            shader->Bind();
            shader->SetUniformMat4("u_Transform", transform);
            shader->Unbind();
        }
        
        Ref<Shader> rendererShader;
        if (isADS)
        {
            rendererShader = s_Data.Shaders.Get("ADS_Shader");
        }
        else
        {
            rendererShader = s_Data.Shaders.Get("LightSourceShader");
            rendererShader->Bind();
            
            glm::vec3 color = {1.0f, 1.0f, 1.0f};
            if (isLightSOurce)
            {
                auto light = s_Data.ActiveLight;
                color = light.Ambient * light.Diffuse * light.Specular;
            }
            
            rendererShader->SetUniformFloat3("u_Color", color);
        }
        
        mesh->Draw(*rendererShader.Raw());
        
        /* skybox cube */
        auto cubeMapShader = s_Data.Shaders.Get("CubeMap");
        cubeMapShader->Bind();
        s_Data.CubeMapData.VertexArray->Bind();
        s_Data.CubeMapData.Texture->Bind();
        
        glm::mat4 cubeMapTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(10.0f));
        cubeMapShader->SetUniformMat4("u_Transform", cubeMapTransform);
        
        Renderer::DrawIndexed(36);
        
        s_Data.CubeMapData.VertexArray->Unbind();
        cubeMapShader->Unbind();
        
        RendererStatistics::VertexCount += 36;
    }
    
}
