#include <iKan/Renderer/API/SceneRenderer.h>
#include <iKan/Renderer/API/Renderer.h>
#include <iKan/Renderer/API/Vertices.h>

#include <iKan/Renderer/Graphics/Shader.h>

namespace iKan {
    
    struct SceneData
    {
        Ref<Shader> MeshShader;
        
        struct CubemapVertex
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
            
            CubemapVertex* VertexBasePtr;
            CubemapVertex* VertexPtr;
        };
        CubeMapData CubeMapData;

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
        
        {
            /* Creating Vertex Array */
            s_Data.CubeMapData.VertexArray = VertexArray::Create();
            
            /* Creating Vertex Buffer and adding Layout */
            s_Data.CubeMapData.VertexBuffer = VertexBuffer::Create( SceneData::CubeMapData::MaxVertex * sizeof(SceneData::CubemapVertex));
            s_Data.CubeMapData.VertexBuffer->AddLayout({
                { ShaderDataType::Float3, "a_Position" }
            });
            s_Data.CubeMapData.VertexArray->AddVertexBuffer(s_Data.CubeMapData.VertexBuffer);
            
            // Allocating the memory for vertex Buffer Pointer
            s_Data.CubeMapData.VertexBasePtr = new SceneData::CubemapVertex[SceneData::CubeMapData::MaxVertex];
            
            /* Shader Bind and texture set*/
            s_Data.CubeMapData.CubeMapShader = Shader::Create("../../Engine/assets/shaders/CubemapShader.glsl");
            s_Data.CubeMapData.CubeMapShader->Bind();
            s_Data.CubeMapData.CubeMapShader->SetUniformInt1("u_Skybox", 0);
        }
    }
    
    void SceneRenderer::SetCubemapTexture(const std::string& path)
    {
        s_Data.CubeMapData.CubeMapTexture = CubeMapTexture::Create(path);
    }
    
    void SceneRenderer::SetupLight(const SceneRendererLight &light)
    {
        s_Data.MeshShader->Bind();
        s_Data.MeshShader->SetUniformInt1("u_IsSceneLight", (light.Light != nullptr));

        if (light.Light)
        {
            // if point light then set the flag to 1
            s_Data.MeshShader->SetUniformInt1("u_PointLight.Present", int((SceneLight::LightType::Point == light.Light->GetType())));
            s_Data.MeshShader->SetUniformInt1("u_SpotLight.Present", int((SceneLight::LightType::Spot == light.Light->GetType())));

            s_Data.MeshShader->SetUniformFloat3("u_Light.Position", light.Position);
            s_Data.MeshShader->SetUniformFloat3("u_ViewPos", light.ViewPos);

            // light properties
            s_Data.MeshShader->SetUniformInt1("u_Light.IsAmbient", light.Light->GetAmbientFlag());
            if (light.Light->GetAmbientFlag())
                s_Data.MeshShader->SetUniformFloat3("u_Light.Ambient", light.Light->GetAmbient());
            
            s_Data.MeshShader->SetUniformInt1("u_Light.IsDiffuse", light.Light->GetDiffuseFlag());
            if (light.Light->GetDiffuseFlag())
                s_Data.MeshShader->SetUniformFloat3("u_Light.Diffuse", light.Light->GetDiffuse());
            
            s_Data.MeshShader->SetUniformInt1("u_Light.IsSpecular", light.Light->GetSpecularFlag());
            if (light.Light->GetSpecularFlag())
                s_Data.MeshShader->SetUniformFloat3("u_Light.Specular", light.Light->GetSpecular());
            
            // Point light variables
            if (SceneLight::LightType::Point == light.Light->GetType())
            {
                s_Data.MeshShader->SetUniformFloat1("u_PointLight.Constant", light.Light->GetConstant());
                s_Data.MeshShader->SetUniformFloat1("u_PointLight.Quadratic", light.Light->GetQuadratic());
                s_Data.MeshShader->SetUniformFloat1("u_PointLight.Linear", light.Light->GetLinear());
            }
            
            // Spot light variables
            else if (SceneLight::LightType::Spot == light.Light->GetType())
            {
                s_Data.MeshShader->SetUniformFloat3("u_SpotLight.Direction", light.CameraFront);

                s_Data.MeshShader->SetUniformFloat1("u_SpotLight.CutOff", glm::cos(glm::radians(light.Light->GetCutoff())));
                s_Data.MeshShader->SetUniformFloat1("u_SpotLight.OuterCutOff", glm::cos(glm::radians(light.Light->GetOuterCutoff())));
            }
            
            // material properties
            // TODO: Add Im gui for this or move to shader hard code
            s_Data.MeshShader->SetUniformFloat1("u_Material.Shininess", 64.0f);
            s_Data.MeshShader->Unbind();
                        
        }
    }
    
    void SceneRenderer::BegineScene(const SceneRendererCamera& camera)
    {
        // Mesh Begin scene
        {
            s_Data.MeshShader->Bind();
            s_Data.MeshShader->SetUniformMat4("u_ViewProjection", camera.Camera.GetProjection() * camera.ViewMatrix);
            s_Data.MeshShader->Unbind();
        }
        
        // Cubemap Begin scene
        {
            s_Data.CubeMapData.CubeMapShader->Bind();
            s_Data.CubeMapData.CubeMapShader->SetUniformMat4("u_ViewProjection", camera.Camera.GetProjection() * camera.ViewMatrix);
            s_Data.CubeMapData.CubeMapShader->Unbind();
        
            s_Data.CubeMapData.VertexPtr = s_Data.CubeMapData.VertexBasePtr;
        }
    }
    
    void SceneRenderer::EndScene()
    {
        
    }
    
    void SceneRenderer::Draw(const Ref<Mesh>& mesh, const glm::mat4& transform)
    {
        if (mesh)
        {
            s_Data.MeshShader->Bind();
            s_Data.MeshShader->SetUniformMat4("u_Transform", transform);
            s_Data.MeshShader->Unbind();
        
            mesh->Draw(*s_Data.MeshShader.Raw());
        }
    }
    
    void SceneRenderer::DrawCubemap(const glm::mat4& transform)
    {
        s_Data.CubeMapData.CubeMapShader->Bind();
        s_Data.CubeMapData.CubeMapShader->SetUniformMat4("u_Transform", transform);
        s_Data.CubeMapData.CubeMapShader->Unbind();
        
        {
            s_Data.CubeMapData.CubeMapTexture->Bind(1);
            s_Data.CubeMapData.CubeMapShader->Bind();
            s_Data.CubeMapData.VertexArray->Bind();
            
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
        }
    }
    
    void SceneRenderer::Shutdown()
    {
        delete[] s_Data.CubeMapData.VertexBasePtr;
    }
    
}
