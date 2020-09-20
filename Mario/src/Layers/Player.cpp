#include <Layers/Player.h>

#include <NativeScripts/PlayerMove.h>

namespace iKan {

    Player* Player::s_Instance = nullptr;
    
    Player::Player(Ref<Scene>& scene)
    {
        IK_INFO("Player Instance Created");

        m_SpriteSheet     = Texture::Create("../../Mario/assets/Resources/Graphics/Player.png");
        m_StandSubtexture = SubTexture::CreateFromCoords(m_SpriteSheet, { 6.0f, m_Color });

        m_Entity = scene->CreateEntity("Player");
        
        m_Entity.GetComponent<TransformComponent>().Transform[3][1] = 10.0f;
        m_Entity.GetComponent<TransformComponent>().Transform[3][0] = 3.0f;
        
        m_Entity.AddComponent<SpriteRendererComponent>(m_StandSubtexture);
        
        m_Entity.AddComponent<NativeScriptComponent>();
        m_Entity.GetComponent<NativeScriptComponent>().Bind<PlayerMove>();
    }
    
    bool Player::OnKeyPressed(KeyPressedEvent& event)
    {
        return false;
    }
    
    bool Player::OnKeyReleased(KeyReleasedEvent& event)
    {
        if (event.GetKeyCode() == Key::Right || event.GetKeyCode() == Key::Left)
        {
            m_MoveIdx = 0;
            m_Entity.GetComponent<SpriteRendererComponent>().SubTexComp = m_StandSubtexture;
        }
        return false;
    }
    
    Player* Player::Create(Ref<Scene>& scene)
    {
        if (s_Instance)
            IK_WARN("Player already created");
        else
            s_Instance = new Player(scene);
        return s_Instance;
    }
    
    void Player::ImGuiRender()
    {
        if (ImGui::CollapsingHeader("Player"))
        {
            if (ImGui::TreeNode("Speed"))
            {
                ImGuiAPI::Counter("Player Speed", m_TranslationSpeed);
                ImGui::TreePop();
            }
            // TODO: Move it outside as it should reset the Game
            if (ImGui::Button("Reset"))
            {
                auto& pos = m_Entity.GetComponent<TransformComponent>().Transform[3];
                // TODO: FIX THIS Debugging only
                pos[0] = 3.0f;
                pos[1] = 15.0f;
            }
        }
        
    }
    
}
