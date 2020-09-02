#include "ExampleLayer.h"

#include <stb_image.h>

namespace iKan {
    
    static bool  s_Perspective = false;
    static float s_AspectRatio = 16.0f/9.0f;
    
    static glm::vec3 s_Position = { 0.0f, 0.0f, 0.0f };
    static glm::vec3 s_Rotation = { 0.0f, 0.0f, 0.0f };
    
    static glm::vec3 s_OrthoCameraPosition = { 0.0f, 0.0f, 0.0f};
    static float     s_OrthoZoom           = 1.0f;
    
    static glm::vec3 s_PerspectivCameraPosition = glm::vec3(0.0f, 0.0f,  3.0f);
    static glm::vec3 s_CameraFront              = glm::vec3(0.0f, 0.0f, -1.0f);
    static glm::vec3 s_CameraLeft               = glm::vec3(-1.0f, 0.0f, 0.0f);
    static glm::vec3 s_CameraUp                 = glm::vec3(0.0f, 1.0f,  0.0f);
    
    static glm::vec3 s_CameraRotation = { 0.0f, 0.0f, 0.0f };
    
    static float s_FOV = 45.0f;
    
    // Light
    struct Light
    {
        glm::vec3 Position = { 1.2f, 1.0f, 2.0f };
        glm::vec3 Diffuse  = { 0.5f, 0.5f, 0.5f };
        glm::vec3 Ambient  = Diffuse * glm::vec3(0.2f, 0.2f, 0.2f);
        glm::vec3 Specular = { 1.0f, 1.0f, 1.0f };
        
        float Constant  = 1.0f;
        float Linear    = 0.09f;
        float Quadratic = 0.032f;
    };
    
    struct Material
    {
        glm::vec3 Diffuse   = { 1.0f, 0.5f, 0.31f };
        glm::vec3 Specular  = { 0.5f, 0.5f, 0.5f };
        float     Shininess = 32.0f;
    };
    
    static Material s_Material;
    static Light    s_Light;
    
    static bool s_IsAmbient     = false;
    static bool s_IsDiffuse     = false;
    static bool s_IsSpecular    = false;
    static bool s_IsAttenuation = false;
    
    ExampleLayer::ExampleLayer()
    : Layer("Example Layer")
    {
        float vertices[]
        {
            -0.5f, -0.5f, 0.0f,     0.1f, 0.3f, 0.5f, 1.0f,      0.0f, 0.0f,     0.0f,    2.0f,     0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, 0.0f,     0.1f, 0.3f, 0.5f, 1.0f,      1.0f, 0.0f,     0.0f,    2.0f,     0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, 0.0f,     0.1f, 0.3f, 0.5f, 1.0f,      1.0f, 1.0f,     0.0f,    2.0f,     0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, 0.0f,     0.1f, 0.3f, 0.5f, 1.0f,      0.0f, 1.0f,     0.0f,    2.0f,     0.0f,  0.0f, -1.0f,
        };
        
        m_VAO = VertexArray::Create();
        
        std::shared_ptr<VertexBuffer> VBO = VertexBuffer::Create(sizeof(vertices), vertices);
        
        VBO->AddLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" },
            { ShaderDataType::Float2, "a_TexCoord" },
            { ShaderDataType::Float , "a_TextureIndex" },
            { ShaderDataType::Float , "a_TilingFactor" },
            { ShaderDataType::Float3, "a_Normal" }
        });
        m_VAO->AddVertexBuffer(VBO);
        
        uint32_t indices[]
        {
            0, 1, 2,
            2, 3, 0
        };
        
        // Creating Vertex Array
        std::shared_ptr<IndexBuffer> EBO  = IndexBuffer::Create(sizeof(indices) / sizeof(uint32_t), indices);
        m_VAO->SetIndexBuffer(EBO);
        
        m_ShaderLibrary.Load("../../Editor/assets/shaders/Texture.glsl");
        
        // Light
        float lightVertices[] =
        {
            -0.5f, -0.5f, 0.0f,     0.1f, 0.3f, 0.5f, 1.0f,      0.0f, 0.0f,     0.0f,    1.0f,
             0.5f, -0.5f, 0.0f,     0.1f, 0.3f, 0.5f, 1.0f,      1.0f, 0.0f,     0.0f,    1.0f,
             0.5f,  0.5f, 0.0f,     0.1f, 0.3f, 0.5f, 1.0f,      1.0f, 1.0f,     0.0f,    1.0f,
            -0.5f,  0.5f, 0.0f,     0.1f, 0.3f, 0.5f, 1.0f,      0.0f, 1.0f,     0.0f,    1.0f,
        };
        
        uint32_t lightIndices[] =
        {
            0, 1, 2,
            2, 3, 0
        };
        
        // Creating Vertex Array
        m_LightVAO = VertexArray::Create();
        std::shared_ptr<VertexBuffer> lightVBO = VertexBuffer::Create(sizeof(lightVertices), lightVertices);
        std::shared_ptr<IndexBuffer> lightEBO  = IndexBuffer::Create(sizeof(lightIndices) / sizeof(uint32_t), lightIndices);
        
        lightVBO->AddLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" },
            { ShaderDataType::Float2, "a_TexCoord" },
            { ShaderDataType::Float , "a_TextureIndex" },
            { ShaderDataType::Float , "a_TilingFactor" },
        });
        m_LightVAO->AddVertexBuffer(lightVBO);
        m_LightVAO->SetIndexBuffer(lightEBO);
        
        m_ShaderLibrary.Load("../../Editor/assets/shaders/Texture.glsl");
        
        auto textureShader = m_ShaderLibrary.Get("Texture");
        textureShader->Bind();
        // Texture
        m_CheckboardTexture = Texture::Create("../../Editor/assets/textures/Checkerboard.png");
        textureShader->SetUniformInt1("u_Texture[1]", 1);
        
        m_GridTexture       = Texture::Create("../../Editor/assets/textures/metal.png");
        textureShader->SetUniformInt1("u_Texture[2]", 2);

        m_GrassTexture      = Texture::Create("../../Editor/assets/textures/grass.png");
        textureShader->SetUniformInt1("u_Texture[3]", 3);
        
        // White Texture
        uint32_t whiteTextureData = 0xffffffff;
        void* whiteTextureDataPtr = &whiteTextureData;
        m_WhiteTexture = Texture::Create(1, 1, whiteTextureDataPtr, sizeof(uint32_t));
        textureShader->SetUniformInt1("u_Texture[0]", 0);
    }
    
    void ExampleLayer::OnAttach()
    {
        
    }
    
    void ExampleLayer::OnDetach()
    {
        
    }
    
    void ExampleLayer::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        
        dispatcher.Dispatch<WindowResizeEvent>(IK_BIND_EVENT_FN(ExampleLayer::OnWindowResize));
        dispatcher.Dispatch<MouseScrollEvent> (IK_BIND_EVENT_FN(ExampleLayer::OnMouseScroll));
    }
    
    bool ExampleLayer::OnWindowResize(WindowResizeEvent& event)
    {
        s_AspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
        return false;
    }
    
    bool ExampleLayer::OnMouseScroll(MouseScrollEvent& event)
    {
        s_CameraRotation.x += event.GetXOffset();
        s_CameraRotation.y += event.GetYOffset();
        
        return false;
    }
                             
    void ExampleLayer::OnUpdate(TimeStep timeStep)
    {
        // Move camera
        const float cameraSpeed = 5.5 * timeStep;
        if (iKan::Input::IsKeyPressed(iKan::Key::Q))
        {
            s_PerspectivCameraPosition += cameraSpeed * s_CameraFront;
            s_OrthoZoom -= cameraSpeed;
            s_OrthoZoom = std::max(s_OrthoZoom, 0.25f);
            
        }
        if (iKan::Input::IsKeyPressed(iKan::Key::E))
        {
            s_PerspectivCameraPosition -= cameraSpeed * s_CameraFront;
            s_OrthoZoom += cameraSpeed;
        }
        if (iKan::Input::IsKeyPressed(iKan::Key::A))
        {
            s_PerspectivCameraPosition -= glm::normalize(glm::cross(s_CameraFront, s_CameraUp)) * cameraSpeed;
            s_OrthoCameraPosition.x -= cameraSpeed;
        }
        if (iKan::Input::IsKeyPressed(iKan::Key::D))
        {
            s_PerspectivCameraPosition += glm::normalize(glm::cross(s_CameraFront, s_CameraUp)) * cameraSpeed;
            s_OrthoCameraPosition.x += cameraSpeed;
        }
        if (iKan::Input::IsKeyPressed(iKan::Key::S))
        {
            s_PerspectivCameraPosition -= glm::normalize(glm::cross(s_CameraFront, s_CameraLeft)) * cameraSpeed;
            s_OrthoCameraPosition.y -= cameraSpeed;
        }
        
        if (iKan::Input::IsKeyPressed(iKan::Key::S))
        {
            s_PerspectivCameraPosition += glm::normalize(glm::cross(s_CameraFront, s_CameraLeft)) * cameraSpeed;
            s_OrthoCameraPosition.y += cameraSpeed;
        }
        
        glm::mat4 projection = glm::mat4(1.0f);
        if(s_Perspective)
            projection = glm::perspective(glm::radians(s_FOV), s_AspectRatio, 0.1f, 100.0f);
        else
            projection = glm::ortho(-s_AspectRatio * s_OrthoZoom, s_AspectRatio * s_OrthoZoom, -s_OrthoZoom, s_OrthoZoom, -100.0f, 100.0f);
        
        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        RenderCommand::Clear();
        
        // Bind Shader
        auto textureShader = m_ShaderLibrary.Get("Texture");
        textureShader->Bind();
        
        // upload model to shader
        glm::mat4 model = glm::translate(glm::mat4(1.0f), s_Position) *
        glm::rotate(glm::mat4(1.0f), glm::radians(s_Rotation.x), glm::vec3(1, 0, 0)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(s_Rotation.y), glm::vec3(0, 1, 0)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(s_Rotation.z), glm::vec3(0, 0, 1));
        
        // Upload Texture to shader at slot 0
        textureShader->SetUniformMat4("u_Model", model);
        
        glm::mat4 view;
        if (s_Perspective)
            view = glm::lookAt(s_PerspectivCameraPosition, s_PerspectivCameraPosition + s_CameraFront, s_CameraUp) *
            glm::rotate(glm::mat4(1.0f), glm::radians(s_CameraRotation.x), glm::vec3(1, 0, 0)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(s_CameraRotation.y), glm::vec3(0, 1, 0)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(s_CameraRotation.z), glm::vec3(0, 0, 1));
        
        else
            view = glm::inverse(glm::translate(glm::mat4(1.0f), s_OrthoCameraPosition) *
                                glm::rotate(glm::mat4(1.0f), glm::radians(s_CameraRotation.x), glm::vec3(1, 0, 0)) *
                                glm::rotate(glm::mat4(1.0f), glm::radians(s_CameraRotation.y), glm::vec3(0, 1, 0)) *
                                glm::rotate(glm::mat4(1.0f), glm::radians(s_CameraRotation.z), glm::vec3(0, 0, 1)));
        
        glm::mat4 projectionView = projection * view;
        textureShader->SetUniformMat4("u_ProjectionView", projectionView);

        // ------------------------------------------------      View Pos   -----------------------------------------
        textureShader->SetUniformFloat3("u_ViewPos", s_PerspectivCameraPosition);
        
        // ------------------------------------------------      Controller -----------------------------------------
        textureShader->SetUniformInt1("u_IsAmbient", (int)s_IsAmbient);
        textureShader->SetUniformInt1("u_IsDiffuse", (int)s_IsDiffuse);
        textureShader->SetUniformInt1("u_IsSpecular", (int)s_IsSpecular);
        textureShader->SetUniformInt1("u_IsAttenuation", (int)s_IsAttenuation);
        
        // -------------------------------------------------      Light     ------------------------------------------
        textureShader->SetUniformFloat3("u_Light.Position", s_Light.Position);

        if (s_IsAmbient)
        {
            textureShader->SetUniformFloat3("u_Light.Ambient", s_Light.Ambient);
            textureShader->SetUniformFloat3("u_Light.Diffuse", s_Light.Diffuse);
        }
        
        if (s_IsDiffuse)
        {
            textureShader->SetUniformFloat3("u_Light.Diffuse", s_Light.Diffuse);
            textureShader->SetUniformFloat3("u_Material.Diffuse", s_Material.Diffuse);
        }
        
        if (s_IsSpecular)
        {
            textureShader->SetUniformFloat3("u_Light.Specular", s_Light.Specular);
            textureShader->SetUniformFloat3("u_Material.Specular", s_Material.Specular);
            textureShader->SetUniformFloat1("u_Material.Shininess", s_Material.Shininess);
        }
        
        if (s_IsAttenuation)
        {
            textureShader->SetUniformFloat1("u_Light.Constant", s_Light.Constant);
            textureShader->SetUniformFloat1("u_Light.Linear", s_Light.Linear);
            textureShader->SetUniformFloat1("u_Light.Quadratic", s_Light.Quadratic);
        }
        
        // Bind Texture
        m_WhiteTexture->Bind(0);
        m_CheckboardTexture->Bind(1);
        m_GrassTexture->Bind(2);
        m_GrassTexture->Bind(3);
        // Draw Element
        RenderCommand::DrawIndexed(m_VAO);
        
        /// Light
        // Bind Shader
        auto lightShader = m_ShaderLibrary.Get("Texture");
        lightShader->Bind();
        
        // upload model to shader
        model = glm::translate(glm::mat4(1.0f), s_Light.Position) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
        
        // Upload Texture to shader at slot 0
        lightShader->SetUniformMat4("u_Model", model);
        lightShader->SetUniformMat4("u_ProjectionView", projectionView);

        // Bind VertexArray
        m_LightVAO->Bind();
        
        // Draw Element
        RenderCommand::DrawIndexed(m_LightVAO);
    }

    void ExampleLayer::OnImguiRender()
    {
        // Render Imgui
        ImGui::Begin("Frame");
        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
        
        ImGui::Begin("Model Camera");
        ImGui::Text("Model Transformation");
        ImGui::DragFloat2("Pos", &s_Position.x);
        ImGui::DragFloat3("Rot", &s_Rotation.x);
        ImGui::Separator();
        
        ImGui::Text("Camera");
        ImGui::Checkbox("Perspective Camera", &s_Perspective);
        ImGui::Separator();
        
        ImGui::DragFloat2("Ortho Pos", &s_OrthoCameraPosition.x);
        ImGui::DragFloat3("Perspective Pos ", &s_PerspectivCameraPosition.x);
        ImGui::DragFloat3("Rotation", &s_CameraRotation.x);
        ImGui::DragFloat("Perspective FOV", &s_FOV);
        ImGui::Separator();
        ImGui::End();
        
        ImGui::Begin("Light and Material");
        ImGui::DragFloat3("Light Pos", &s_Light.Position.x);
        ImGui::Separator();
        
        ImGui::Checkbox("Ambient", &s_IsAmbient);
        ImGui::ColorEdit3("Light Ambient", &s_Light.Ambient.r);
        ImGui::Separator();
        
        ImGui::Checkbox("Diffuse", &s_IsDiffuse);
        ImGui::ColorEdit3("Light Diffuse", &s_Light.Diffuse.r);
        ImGui::ColorEdit3("Material Diffuse", &s_Material.Diffuse.r);
        ImGui::Separator();
        
        ImGui::Checkbox("Specular", &s_IsSpecular);
        ImGui::ColorEdit3("Light Specular", &s_Light.Specular.r);
        ImGui::ColorEdit3("Material Specular", &s_Material.Specular.r);
        ImGui::SliderFloat("Shininess", &s_Material.Shininess, 0.0f, 128.0f);
        ImGui::Separator();
        
        ImGui::Checkbox("Attenuation (Point Light)", &s_IsAttenuation);
        ImGui::SliderFloat("Light Constant", &s_Light.Constant, 0.0f, 10.0f);
        ImGui::SliderFloat("Light Linear", &s_Light.Linear, 0.0f, 0.1f);
        ImGui::SliderFloat("Light Quadratic", &s_Light.Quadratic, 0.0f, 0.05f);
        ImGui::Separator();
        
        //        if(ImGui::ImageButton((void*)(size_t)m_CheckBoardTextureID, ImVec2{ 100.0f, 100.0f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 }))
        //        {
        //            // TODO: will add after batch rendering
        //        }
        
        ImGui::End();
    }
    
}
