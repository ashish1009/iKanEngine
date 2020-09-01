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
        
        std::string vertexSrc = R"(
        #version 330 core
        
        layout (location = 0) in vec3  a_Position;
        layout (location = 1) in vec4  a_Color;
        layout (location = 2) in vec2  a_TexCoord;
        layout (location = 3) in float a_TextureIndex;
        layout (location = 4) in float a_TilingFactor;
        layout (location = 5) in vec3  a_Normal;
        
        out vec3  v_Position;
        out vec4  v_Color;
        out vec2  v_TexCoord;
        out float v_TextureIndex;
        out float v_TilingFactor;
        out vec3  v_Normal;
        
        uniform mat4 u_Model;
        uniform mat4 u_ProjectionView;
        
        void main()
        {
        v_Position     = vec3(u_Model * vec4(a_Position, 1.0));
        v_Color        = a_Color;
        v_TexCoord     = a_TexCoord;
        v_TextureIndex = a_TextureIndex;
        v_TilingFactor = a_TilingFactor;
        v_Normal       = a_Normal;
        
        gl_Position = u_ProjectionView * u_Model * vec4(a_Position.x, a_Position.y, a_Position.z, 1.0);
        }
        )";
        
        std::string fragmentSrc = R"(
        #version 330 core
        
        out vec4 FragColor;
        
        struct Material
        {
        vec3  Diffuse;
        vec3  Specular;
        float Shininess;
        };
        
        struct Light
        {
        vec3 Position;
        vec3 Ambient;
        vec3 Diffuse;
        vec3 Specular;
        
        float Constant;
        float Linear;
        float Quadratic;
        };
        
        in vec3  v_Position;
        in vec4  v_Color;
        in vec2  v_TexCoord;
        in float v_TextureIndex;
        in float v_TilingFactor;
        in vec3  v_Normal;
        
        uniform sampler2D u_Texture[16];
        uniform Material  u_Material;
        uniform Light     u_Light;
        uniform vec3      u_ViewPos;
        
        uniform int u_IsAmbient;
        uniform int u_IsDiffuse;
        uniform int u_IsSpecular;
        uniform int u_IsAttenuation;
        
        void LightProp(vec3 lightDir)
        {
        vec4 result = vec4(0.0f, 0.0f, 0.0f, 0.0f);
        
        vec3  norm          = normalize(v_Normal);
        float attenuation   = 0.0f;
        
        // Attenuation
        if (1 == int(u_IsAttenuation))
        {
        float distance = length(lightDir);
        attenuation    = 1.0 / (u_Light.Constant + u_Light.Linear * distance + u_Light.Quadratic * (distance * distance));
        }
        
        // ambient
        if (1 == int(u_IsAmbient))
        {
        vec3 ambient = u_Light.Ambient * u_Material.Diffuse;
        
        if (1 == int(u_IsAttenuation))
        ambient *= attenuation;
        
        result += vec4(ambient, 1.0f);
        }
        
        // diffuse
        if (1 == int(u_IsDiffuse))
        {
        float diff    = max(dot(norm, lightDir), 0.0);
        vec3  diffuse = u_Light.Diffuse * (diff * u_Material.Diffuse);
        
        if (1 == int(u_IsAttenuation))
        diffuse  *= attenuation;
        
        result += vec4(diffuse, 1.0f);
        }
        
        // specular
        if (1 == int(u_IsSpecular))
        {
        vec3 viewDir    = normalize(u_ViewPos - v_Position);
        vec3 reflectDir = reflect(-lightDir, norm);
        
        float spec      = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.Shininess);
        vec3  specular  = u_Light.Specular * (spec * u_Material.Specular);
        
        if (1 == int(u_IsAttenuation))
        specular *= attenuation;
        
        result += vec4(specular, 1.0f);
        }
        
        if (0 == int(u_IsAmbient) && 0 == int(u_IsDiffuse) && 0 == int(u_IsSpecular))
        result = vec4(1.0f, 1.0f, 1.0f, 1.0f);
        
        vec4 Color = texture(u_Texture[int(v_TextureIndex)], v_TexCoord * v_TilingFactor) * v_Color * result;
        if (Color.a < 0.1)
        discard;
        FragColor = Color;
        }
        
        void main()
        {
        vec3  lightDir = normalize(u_Light.Position - v_Position);
        
        LightProp(lightDir);
        }
        )";
        
        m_Shader = Shader::Create(vertexSrc, fragmentSrc);
        
        /// Light
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
        
        m_LightShader = Shader::Create(vertexSrc, fragmentSrc);
        
        m_Shader->Bind();
        // Texture
        m_CheckboardTexture = Texture::Create("../../Editor/assets/textures/Checkerboard.png");
        m_Shader->SetUniformInt1("u_Texture[1]", 1);
        
        m_GridTexture       = Texture::Create("../../Editor/assets/textures/metal.png");
        m_Shader->SetUniformInt1("u_Texture[2]", 2);

        m_GrassTexture      = Texture::Create("../../Editor/assets/textures/grass.png");
        m_Shader->SetUniformInt1("u_Texture[3]", 3);
        
        // White Texture
        uint32_t whiteTextureData = 0xffffffff;
        void* whiteTextureDataPtr = &whiteTextureData;
        m_WhiteTexture = Texture::Create(1, 1, whiteTextureDataPtr, sizeof(uint32_t));
        m_Shader->SetUniformInt1("u_Texture[0]", 0);
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
        m_Shader->Bind();
        
        // upload model to shader
        glm::mat4 model = glm::translate(glm::mat4(1.0f), s_Position) *
        glm::rotate(glm::mat4(1.0f), glm::radians(s_Rotation.x), glm::vec3(1, 0, 0)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(s_Rotation.y), glm::vec3(0, 1, 0)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(s_Rotation.z), glm::vec3(0, 0, 1));
        
        // Upload Texture to shader at slot 0
        m_Shader->SetUniformMat4("u_Model", model);
        
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
        m_Shader->SetUniformMat4("u_ProjectionView", projectionView);

        // ------------------------------------------------      View Pos   -----------------------------------------
        m_Shader->SetUniformFloat3("u_ViewPos", s_PerspectivCameraPosition);
        
        // ------------------------------------------------      Controller -----------------------------------------
        m_Shader->SetUniformInt1("u_IsAmbient", (int)s_IsAmbient);
        m_Shader->SetUniformInt1("u_IsDiffuse", (int)s_IsDiffuse);
        m_Shader->SetUniformInt1("u_IsSpecular", (int)s_IsSpecular);
        m_Shader->SetUniformInt1("u_IsAttenuation", (int)s_IsAttenuation);
        
        // -------------------------------------------------      Light     ------------------------------------------
        m_Shader->SetUniformFloat3("u_Light.Position", s_Light.Position);

        if (s_IsAmbient)
        {
            m_Shader->SetUniformFloat3("u_Light.Ambient", s_Light.Ambient);
            m_Shader->SetUniformFloat3("u_Light.Diffuse", s_Light.Diffuse);
        }
        
        if (s_IsDiffuse)
        {
            m_Shader->SetUniformFloat3("u_Light.Diffuse", s_Light.Diffuse);
            m_Shader->SetUniformFloat3("u_Material.Diffuse", s_Material.Diffuse);
        }
        
        if (s_IsSpecular)
        {
            m_Shader->SetUniformFloat3("u_Light.Specular", s_Light.Specular);
            m_Shader->SetUniformFloat3("u_Material.Specular", s_Material.Specular);
            m_Shader->SetUniformFloat1("u_Material.Shininess", s_Material.Shininess);
        }
        
        if (s_IsAttenuation)
        {
            m_Shader->SetUniformFloat1("u_Light.Constant", s_Light.Constant);
            m_Shader->SetUniformFloat1("u_Light.Linear", s_Light.Linear);
            m_Shader->SetUniformFloat1("u_Light.Quadratic", s_Light.Quadratic);
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
        m_LightShader->Bind();
        
        // upload model to shader
        model = glm::translate(glm::mat4(1.0f), s_Light.Position) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
        
        // Upload Texture to shader at slot 0
        m_LightShader->SetUniformMat4("u_Model", model);
        m_LightShader->SetUniformMat4("u_ProjectionView", projectionView);

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
