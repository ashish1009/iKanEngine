#include <iKan/Renderer/SceneRenderer.h>
#include <iKan/Renderer/Renderer.h>
#include <iKan/Renderer/Shader.h>
#include <iKan/Renderer/VertexArray.h>
#include <iKan/Renderer/Buffers.h>

namespace iKan {
    
    struct SceneRendererData
    {
        ShaderLibrary    ShaderLibrary;
        Ref<VertexArray> VertexArray;
    };
    static SceneRendererData s_Data;
    
    static glm::vec3 s_PerspectivCameraPosition = glm::vec3(0.0f, 0.0f,  3.0f);
    static glm::vec3 s_CameraFront              = glm::vec3(0.0f, 0.0f, -1.0f);
    static glm::vec3 s_CameraUp                 = glm::vec3(0.0f, 1.0f,  0.0f);
    
    void SceneRenderer::Init()
    {
        s_Data.VertexArray = VertexArray::Create();

        float vertices[]
        {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.5f,  0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f
        };

        Ref<VertexBuffer> VBO = VertexBuffer::Create(sizeof(vertices), vertices);
        
        VBO->AddLayout({
            { ShaderDataType::Float3, "a_Position" },
        });
        s_Data.VertexArray->AddVertexBuffer(VBO);
        
        uint32_t indices[]
        {
            0, 1, 2,
            2, 3, 0
        };
        
        // Creating Vertex Array
        Ref<IndexBuffer> EBO  = IndexBuffer::Create(sizeof(indices) / sizeof(uint32_t), indices);
        s_Data.VertexArray->SetIndexBuffer(EBO);
        
        s_Data.ShaderLibrary.Load("../../Editor/assets/shaders/CommonShader.glsl");
    }
    
    void SceneRenderer::BeginScene(const Camera& camera, const glm::mat4& transform)
    {
    }
    
    void SceneRenderer::DrawCube()
    {
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), 3.4f, 0.1f, 100.0f);
        
        auto textureShader = s_Data.ShaderLibrary.Get("CommonShader");
        textureShader->Bind();
        
        // upload model to shader
        glm::mat4 model = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 0.0f });
        // Upload Texture to shader at slot 0
        textureShader->SetUniformMat4("u_Model", model);
        
        glm::mat4 view;
        view = glm::lookAt(s_PerspectivCameraPosition, s_PerspectivCameraPosition + s_CameraFront, s_CameraUp);
        
        glm::mat4 projectionView = projection * view;
        textureShader->SetUniformMat4("u_ProjectionView", projectionView);
        
        // Draw Element
        Renderer::DrawIndexed(s_Data.VertexArray);
    }
    
    void SceneRenderer::EndScene()
    {
    }
    
    void SceneRenderer::Shutdown()
    {
    }
    
}
