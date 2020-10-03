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
        
        Ref<Mesh> bagMesh = Ref<Mesh>::Create("../../Editor/assets/resources/objects/backpack/backpack.obj");
        auto bagEntity    = m_ActiveScene->CreateEntity("Bag");
        bagEntity.AddComponent<MeshComponent>(bagMesh);
        
        bagEntity.GetComponent<TransformComponent>().Transform[3][0] = 3;
        
        Ref<Mesh> moonMesh = Ref<Mesh>::Create("../../Editor/assets/resources/objects/Moon/Moon.obj");
        auto moonEntity    = m_ActiveScene->CreateEntity("Moon");
        moonEntity.AddComponent<MeshComponent>(moonMesh).ADS = false;
        
        Ref<Mesh> lightMesh = Ref<Mesh>::Create("../../Editor/assets/resources/objects/Light/Light.obj");
        m_LightEntity       = m_ActiveScene->CreateEntity("Light Source");
        m_LightEntity.AddComponent<MeshComponent>(lightMesh).ADS = false;

        auto& transform = m_LightEntity.GetComponent<TransformComponent>().Transform;
        transform       = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f)) * glm::toMat4(glm::quat(glm::radians(glm::vec3(0.0f)))) * glm::scale(glm::mat4(1.0f), glm::vec3(0.2));
        
        Ref<Mesh> pokemonMesh = Ref<Mesh>::Create("../../Editor/assets/resources/objects/pokemon/Pokemon.obj");
        auto pokemonEntity    = m_ActiveScene->CreateEntity("Pokemon");
        pokemonEntity.AddComponent<MeshComponent>(pokemonMesh);
        
        pokemonEntity.GetComponent<TransformComponent>().Transform[3][0] = -3;
        
        Ref<Mesh> groundMesh = Ref<Mesh>::Create("../../Editor/assets/resources/objects/GroundPlane/GroundPlane.obj");
        auto groundEntity    = m_ActiveScene->CreateEntity("Ground");
        groundEntity.AddComponent<MeshComponent>(groundMesh);
        
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
        
        m_ActiveScene->OnEditorUpdate(timeStep, m_EditorCamera);
        
        m_FrameBuffer->Unbind();
    }
    
    void SceneEditor::OnImguiRender()
    {
        ImGuiAPI::EnableDcocking();
        
        {
            ImGui::Begin("Setting");
            if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Light"))
            {
                auto& light = m_ActiveScene->GetLight();
                
                ImGui::Columns(2);
                ImGui::Text("Position");
                ImGui::NextColumn();
                ImGui::PushItemWidth(-1);

                ImGui::DragFloat3("##Position", &light.Position.x);
                
                auto& lightPosition = m_LightEntity.GetComponent<TransformComponent>().Transform[3];
                lightPosition[0] = light.Position.x;
                lightPosition[1] = light.Position.y;
                lightPosition[2] = light.Position.z;
                
                ImGui::PopItemWidth();
                ImGui::NextColumn();
                ImGui::Columns(1);

                ImGui::Checkbox("IsAmbient", &light.LightFlag.IsAmbient);
                if (light.LightFlag.IsAmbient)
                {
                    ImGui::Columns(2);
                    ImGui::Text("Ambient");
                    ImGui::NextColumn();
                    ImGui::PushItemWidth(-1);
                    
                    ImGui::DragFloat3("##Ambient", &light.Ambient.x);
                    
                    ImGui::PopItemWidth();
                    ImGui::NextColumn();
                    ImGui::Columns(1);
                }
                
                ImGui::Checkbox("IsDiffuse", &light.LightFlag.IsDiffuse);
                if (light.LightFlag.IsDiffuse)
                {
                    ImGui::Columns(2);
                    ImGui::Text("Diffuse");
                    ImGui::NextColumn();
                    ImGui::PushItemWidth(-1);
                    
                    ImGui::DragFloat3("##Diffuse", &light.Diffuse.x);
                    
                    ImGui::PopItemWidth();
                    ImGui::NextColumn();
                    ImGui::Columns(1);
                }
                
                ImGui::Checkbox("IsSpecular", &light.LightFlag.IsSpecular);
                if (light.LightFlag.IsSpecular)
                {
                    ImGui::Columns(2);
                    ImGui::Text("Specular");
                    ImGui::NextColumn();
                    ImGui::PushItemWidth(-1);
                    
                    ImGui::DragFloat3("##Specular", &light.Specular.x);
                    
                    ImGui::PopItemWidth();
                    ImGui::NextColumn();
                    ImGui::Columns(1);
                }
                
                ImGui::TreePop();
            }
            ImGui::End();
        }
        
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
