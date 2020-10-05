#include <iKan/Renderer/RenderCubeMap.h>

#include <iKan/Renderer/VertexArray.h>
#include <iKan/Renderer/Buffers.h>
#include <iKan/Renderer/Texture.h>
#include <iKan/Renderer/Shader.h>
#include <iKan/Renderer/Renderer.h>

namespace iKan {
    
    struct CubeMapData
    {
        Ref<VertexArray>    VertexArray;
        Ref<VertexBuffer>   VertexBuffer;
        Ref<CubeMapTexture> Texture;
        Ref<Shader>         Shader;
    };
    static CubeMapData s_CubeMapData;
    
    void RenderCubeMap::Init()
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
        s_CubeMapData.VertexArray = VertexArray::Create();
        
        /* Creating Vertex Buffer and adding Layout */
        s_CubeMapData.VertexBuffer = VertexBuffer::Create(sizeof(skyboxVertices), skyboxVertices);
        s_CubeMapData.VertexBuffer->AddLayout({
            { ShaderDataType::Float3, "a_Position" }
        });
        s_CubeMapData.VertexArray->AddVertexBuffer(s_CubeMapData.VertexBuffer);
        
        /* Shader Bind and texture set*/
        s_CubeMapData.Shader->Bind();
        s_CubeMapData.Shader->SetUniformInt1("u_Skybox", 0);
    }
    
    void RenderCubeMap::BeginScene(const Camera &camera, const glm::mat4 &transform)
    {
        s_CubeMapData.Shader->SetUniformMat4("u_ProjectionView", camera.GetProjection() * transform);
    }
    
    void RenderCubeMap::SetShaader(const std::string &cubeMapPath)
    {
        s_CubeMapData.Shader  = Shader::Create(cubeMapPath);
    }

    void RenderCubeMap::SetCubeMapTexture(const std::vector<std::string>& paths)
    {
        s_CubeMapData.Texture = CubeMapTexture::Create(paths);
    }
    
    void RenderCubeMap::DrawCube()
    {
        /* skybox cube */
        s_CubeMapData.Shader->Bind();
        s_CubeMapData.VertexArray->Bind();
        s_CubeMapData.Texture->Bind();
        
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(10.0f));
//        s_CubeMapData.Shader->SetUniformMat4("u_Transform", transform);
        
        Renderer::DrawIndexed(36);
        
        s_CubeMapData.VertexArray->Unbind();
        s_CubeMapData.Shader->Unbind();

    }

}
