#include "SceneEditorLayer.h"

namespace iKan {
    
    SceneCamera camera;

    SceneEditor::SceneEditor()
    : m_EditorCamera(glm::radians(45.0f), 1800.0f/800.0f, 0.01f, 10000.0f)
    {
    }
    
    SceneEditor::~SceneEditor()
    {
    }
    
    void SceneEditor::OnAttach()
    {
        ImGuiAPI::LightGreyBackground();
        
        FramebufferSpecification specs;
        specs.Width  = s_WindowWidth;
        specs.Height = s_WindowWidth;
        
        m_FrameBuffer = Framebuffer::Create(specs);
        
        m_ActiveScene = Ref<Scene>::Create(Scene::SceneRendererType::_3D);
        
        {
            std::vector<std::string> faces =
            {
                "../../Editor/assets/resources/textures/skybox/right.jpg",
                "../../Editor/assets/resources/textures/skybox/left.jpg",
                "../../Editor/assets/resources/textures/skybox/top.jpg",
                "../../Editor/assets/resources/textures/skybox/bottom.jpg",
                "../../Editor/assets/resources/textures/skybox/front.jpg",
                "../../Editor/assets/resources/textures/skybox/back.jpg"
            };
            SceneRenderer::SetCubeMapTexture(faces);
        }
        
        auto lightEntity = m_ActiveScene->CreateEntity("Light");
        lightEntity.AddComponent<LightComponent>();
        
        Ref<Mesh> lightMesh = Ref<Mesh>::Create("../../Editor/assets/resources/objects/Sphere/Sphere.obj");
        lightEntity.AddComponent<MeshComponent>(lightMesh);
        lightEntity.GetComponent<MeshComponent>().Prop = MeshComponent::Property::LightSource;
        lightEntity.GetComponent<TransformComponent>().Transform = GlmMath::SetTransfrom({ -5.0f, 0.0f, 0.0f }, glm::vec3(0.0f), glm::vec3(0.2f));

        std::unordered_map<std::string, Ref<Mesh>> meshMap;
        meshMap["GlassShaper"] = Ref<Mesh>::Create("../../Editor/assets/resources/objects/Sphere/Sphere.obj");
        meshMap["Bag"]      = Ref<Mesh>::Create("../../Editor/assets/resources/objects/backpack/backpack.obj");
//        meshMap["Moon"]     = Ref<Mesh>::Create("../../Editor/assets/resources/objects/Moon/Moon.obj");
//        meshMap["Pokemon"]  = Ref<Mesh>::Create("../../Editor/assets/resources/objects/pokemon/Pokemon.obj");
//        meshMap["Ground"]   = Ref<Mesh>::Create("../../Editor/assets/resources/objects/Plane/GroundPlane.obj");
        
        for (auto kv : meshMap)
        {
            auto name = kv.first;
            auto mesh = kv.second;
            
            m_EntityMap[name] = m_ActiveScene->CreateEntity(name);
            m_EntityMap[name].AddComponent<MeshComponent>(mesh);
            
            if (name == "GlassShaper")
            {
                m_EntityMap[name].GetComponent<MeshComponent>().Prop = MeshComponent::Property::Reflection;
                m_EntityMap[name].GetComponent<TransformComponent>().Transform = GlmMath::SetTransfrom({ 5.0f, 0.0f, 5.0f }, glm::vec3(0.0f), glm::vec3(5.0f));
            }
            
            if (name == "Bag")
            {
                m_EntityMap[name].GetComponent<TransformComponent>().Transform = GlmMath::SetTransfrom({ -5.0f, 0.0f, 0.0f }, glm::vec3(0.0f), glm::vec3(0.5f));
            }
            
            if (name == "Moon")
            {
                m_EntityMap[name].GetComponent<TransformComponent>().Transform = GlmMath::SetTransfrom({ 0.0f, 0.0f, 10.0f }, glm::vec3(0.0f), glm::vec3(0.2f));
            }
            
            if (name == "Pokemon")
            {
                m_EntityMap[name].GetComponent<TransformComponent>().Transform = GlmMath::SetTransfrom({ 5.0f, 0.0f, 0.0f }, glm::vec3(0.0f), glm::vec3(0.5f));
            }

            if (name == "Ground")
            {
                m_EntityMap[name].GetComponent<TransformComponent>().Transform = GlmMath::SetTransfrom({ 0.0f, -1.0f, 0.0f }, glm::vec3(0.0f), glm::vec3(50.0f));
            }

        }
                        
        m_SceneHierarchyPannel.SetContext(m_ActiveScene);
    }
    
    void SceneEditor::OnDetach()
    {
    }
    
    void SceneEditor::OnEvent(Event& event)
    {
        m_EditorCamera.OnEvent(event);
    }
    
    void SceneEditor::OnUpdate(TimeStep timeStep)
    {
        // If resize the window call the update the Scene View port and Frame buffer should be resized
        if (FramebufferSpecification spec = m_FrameBuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_EditorCamera.SetViewportSize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }
        
        m_EditorCamera.OnUpdate(timeStep);
        
        RendererStatistics::Reset();
        m_FrameBuffer->Bind();
        
        Renderer::Clear({ 0.2, 0.2, 0.2, 1.0f });
        
        camera.SetViewportSize(1600.0f, 800.0f);
        
//        m_ActiveScene->SetLightPosition(m_EntityMap["Light"].GetComponent<TransformComponent>().Transform);
        m_ActiveScene->OnEditorUpdate(timeStep, m_EditorCamera);
        
        m_FrameBuffer->Unbind();
    }
    
    void SceneEditor::OnImguiRender()
    {
        ImGuiAPI::EnableDcocking();
        
        //------------------------ Stats and Version  ------------------------------------------------------
        ImGuiAPI::StatsAndFrameRate((ImGuiRendererType)m_ActiveScene->GetRendererType());
        ImGuiAPI::RendererVersion();
        
        //------------------------ SceneHierarchy Pannel  --------------------------------------------------
        m_SceneHierarchyPannel.OnImguiender();
        
        //------------------------ View Port ---------------------------------------------------------------
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Viewport");
        
        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);
        
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        
        size_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
        ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::End();
        ImGui::PopStyleVar();
        
        ImGuiAPI::EndDocking();
    }
    
}
