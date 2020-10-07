#include <iKan/Renderer/SceneRenderer.h>
#include <iKan/Renderer/Renderer.h>
#include <iKan/Renderer/Shader.h>
#include <iKan/Renderer/VertexArray.h>
#include <iKan/Renderer/Buffers.h>
#include <iKan/Renderer/Texture.h>
#include <iKan/Renderer/RenderStats.h>
#include <iKan/Renderer/Mesh.h>
#include <iKan/Renderer/Vertices.h>

namespace iKan {
    
    struct SceneRendererData
    {
        static const uint32_t MaxTextureSlots = 16;
        
        struct Vertex
        {
            glm::vec3 Position;
        };

        struct CubeMapData
        {
            static const uint32_t MaxVertex = 20000;
            
            Ref<VertexArray>    VertexArray;
            Ref<VertexBuffer>   VertexBuffer;
            Ref<CubeMapTexture> CubeMapTexture;
            Ref<Shader>         CubeMapShader;
            
            Vertex* VertexBasePtr;
            Vertex* VertexPtr;
        };
        CubeMapData CubeMapData;
        
        SceneRendererCamera SceneCamera;
        Light ActiveLight;
        
        ShaderLibrary Shaders;
        Ref<Shader>   LightSourceShader;
        Ref<Shader>   ADS_Shader;
        Ref<Shader>   EnvironmentMapShader;
        
        Ref<Texture> WhiteTexture;
    };
    static SceneRendererData s_Data;
    
    void SceneRenderer::InitMeshData()
    {
        // Creating array of Slots to store hem in shader
        int32_t samplers[s_Data.MaxTextureSlots];
        for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
            samplers[i] = i;
        
        // Creating Shader and storing all the slots
        s_Data.LightSourceShader->Bind();
        s_Data.LightSourceShader->SetUniformInt1("u_Texture", 0);
        s_Data.LightSourceShader->Unbind();
        
        s_Data.ADS_Shader->Bind();
        s_Data.ADS_Shader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);
        s_Data.ADS_Shader->Unbind();
        
        // Creating whote texture for cololful quads witout any texture or sprite
        uint32_t whiteTextureData = 0xffffffff;
        s_Data.WhiteTexture       = Texture::Create(1, 1, &whiteTextureData, sizeof(uint32_t));
    }
    
    void SceneRenderer::InitCubeMapData()
    {
        /* Creating Vertex Array */
        s_Data.CubeMapData.VertexArray = VertexArray::Create();
        
        /* Creating Vertex Buffer and adding Layout */
        s_Data.CubeMapData.VertexBuffer = VertexBuffer::Create( SceneRendererData::CubeMapData::MaxVertex * sizeof(SceneRendererData::Vertex));
        s_Data.CubeMapData.VertexBuffer->AddLayout({
            { ShaderDataType::Float3, "a_Position" }
        });
        s_Data.CubeMapData.VertexArray->AddVertexBuffer(s_Data.CubeMapData.VertexBuffer);
        
        // Allocating the memory for vertex Buffer Pointer
        s_Data.CubeMapData.VertexBasePtr = new SceneRendererData::Vertex[SceneRendererData::CubeMapData::MaxVertex];
        
        /* Shader Bind and texture set*/
        s_Data.CubeMapData.CubeMapShader->Bind();
        s_Data.CubeMapData.CubeMapShader->SetUniformInt1("u_Skybox", 0);
        
        s_Data.EnvironmentMapShader->Bind();
        s_Data.EnvironmentMapShader->SetUniformInt1("u_Skybox", 0);
    }
    
    void SceneRenderer::Init()
    {
        //TODO: Load Shader from outside or move the files inside Engine
        s_Data.ADS_Shader                = s_Data.Shaders.Load("../../Editor/assets/shaders/ADS_Shader.glsl");
        s_Data.LightSourceShader         = s_Data.Shaders.Load("../../Editor/assets/shaders/LightSourceShader.glsl");
        s_Data.CubeMapData.CubeMapShader = s_Data.Shaders.Load("../../Editor/assets/shaders/CubeMapShader.glsl");
        s_Data.EnvironmentMapShader      = s_Data.Shaders.Load("../../Editor/assets/shaders/EnvironmentMapShader.glsl");
        
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
        
        s_Data.EnvironmentMapShader->Bind();
        s_Data.EnvironmentMapShader->SetUniformFloat3("u_CameraPos", camera.Camera.GetProjection()[3]);

        {
            s_Data.CubeMapData.VertexPtr = s_Data.CubeMapData.VertexBasePtr;
        }

        
        // TODO: Try to move these somewhere else
        // Upload uniforms
        s_Data.ADS_Shader->Bind();
        s_Data.ADS_Shader->SetUniformFloat3("u_Light.Position", lightProp.Position);
        s_Data.ADS_Shader->SetUniformFloat3("u_ViewPos", s_Data.SceneCamera.ViewMatrix[3]);
        
        // light properties
        s_Data.ADS_Shader->SetUniformInt1("u_LightFlag.IsAmbient", lightProp.LightFlag.IsAmbient);
        if (lightProp.LightFlag.IsAmbient)
            s_Data.ADS_Shader->SetUniformFloat3("u_Light.Ambient", lightProp.Ambient);
        
        s_Data.ADS_Shader->SetUniformInt1("u_LightFlag.IsDiffuse", lightProp.LightFlag.IsDiffuse);
        if (lightProp.LightFlag.IsAmbient)
            s_Data.ADS_Shader->SetUniformFloat3("u_Light.Diffuse", lightProp.Diffuse);
        
        s_Data.ADS_Shader->SetUniformInt1("u_LightFlag.IsSpecular", lightProp.LightFlag.IsSpecular);
        if (lightProp.LightFlag.IsAmbient)
            s_Data.ADS_Shader->SetUniformFloat3("u_Light.Specular", lightProp.Specular);
        
        s_Data.ADS_Shader->SetUniformInt1("u_LightFlag.IsAttenuation", lightProp.LightFlag.IsAttenuation);
        if (lightProp.LightFlag.IsAttenuation)
        {
            s_Data.ADS_Shader->SetUniformFloat1("u_Light.Constant", lightProp.Constant);
            s_Data.ADS_Shader->SetUniformFloat1("u_Light.Linear", lightProp.Linear);
            s_Data.ADS_Shader->SetUniformFloat1("u_Light.Quadratic", lightProp.Quadratic);
        }
        
        s_Data.ADS_Shader->SetUniformInt1("u_LightFlag.IsSpotLight", lightProp.LightFlag.IsSpotLight);
        if (lightProp.LightFlag.IsSpotLight)
        {
            s_Data.ADS_Shader->SetUniformFloat3("u_Light.Direction", camera.Front);
            s_Data.ADS_Shader->SetUniformFloat1("u_Light.CutOff", glm::cos(glm::radians(lightProp.CutOff)));
        }
        
        // material properties
        s_Data.ADS_Shader->SetUniformFloat1("u_Material.Shininess", 64.0f);
        s_Data.ADS_Shader->Unbind();
        
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
        s_Data.CubeMapData.CubeMapTexture = CubeMapTexture::Create(paths);
        RendererStatistics::TextureCount += 6;
    }
    
    void SceneRenderer::DrawMesh(const Ref<Mesh>& mesh, const glm::mat4& transform, const MeshComponent::Property& meshProp)
    {
        for (auto kv : s_Data.Shaders.GetShaders())
        {
            auto shader = kv.second;
            if (shader->GetName() != "CubeMapShader")
            {
                shader->Bind();
                shader->SetUniformMat4("u_Transform", transform);
                shader->Unbind();
            }
        }
        
        Ref<Shader> rendererShader;
        switch (meshProp)
        {
            case MeshComponent::Property::Basic:
            {
                rendererShader = s_Data.LightSourceShader;
                rendererShader->Bind();
                
                glm::vec3 color = {1.0f, 1.0f, 1.0f};
                rendererShader->SetUniformFloat3("u_Color", color);
                break;
            }
                
            case MeshComponent::Property::ADS:
            {
                rendererShader = s_Data.ADS_Shader;
                break;
            }

            case MeshComponent::Property::LightSource:
            {
                rendererShader = s_Data.LightSourceShader;
                rendererShader->Bind();
                
                auto light = s_Data.ActiveLight;
                glm::vec3 color = light.Ambient * light.Diffuse * light.Specular;
        
                rendererShader->SetUniformFloat3("u_Color", color);
                break;
            }

            case MeshComponent::Property::Reflection:
            {
                rendererShader = s_Data.EnvironmentMapShader;
                break;
            }

            default:
                break;
        }
        
        mesh->Draw(*rendererShader.Raw());
        
        /* skybox cube */
        s_Data.CubeMapData.CubeMapShader->Bind();
        s_Data.CubeMapData.VertexArray->Bind();
        s_Data.CubeMapData.CubeMapTexture->Bind();
        
        glm::mat4 cubeMapTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1000.0f));
        
        for (uint32_t i = 0; i < 36; i++)
        {
            s_Data.CubeMapData.VertexPtr->Position = cubeMapTransform * Vertices::Cube::Data[i];
            s_Data.CubeMapData.VertexPtr++;
        }
        
        // getting the Number of vertex but subtracting the running and base pointers
        uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CubeMapData.VertexPtr - (uint8_t*)s_Data.CubeMapData.VertexBasePtr);
        s_Data.CubeMapData.VertexBuffer->SetData(s_Data.CubeMapData.VertexBasePtr, dataSize);
        
        Renderer::DrawIndexed(36);
        
        s_Data.CubeMapData.VertexArray->Unbind();
        s_Data.CubeMapData.CubeMapShader->Unbind();
        
        RendererStatistics::VertexCount += 36;
    }
    
}
