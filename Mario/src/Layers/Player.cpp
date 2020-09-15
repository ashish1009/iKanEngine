#include <Layers/Player.h>

#include <NativeScripts/PlayerMove.h>

namespace iKan {

    Player* Player::s_Instance = nullptr;
    
    Player::Player(const std::shared_ptr<Scene>& scene)
    {
        IK_INFO("Player Instance Created");

        m_SpriteSheet     = Texture::Create("../../Mario/assets/Resources/Graphics/Player.png");
        m_StandSubtexture = SubTexture::CreateFromCoords(m_SpriteSheet, { 6.0f, m_Color });

        m_Entity = scene->CreateEntity("Player");
        m_Entity.GetComponent<TransformComponent>().Transform[3][1] = 10.0f;
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
    
    Player* Player::Create(const std::shared_ptr<Scene>& scene)
    {
        if (s_Instance)
            IK_WARN("Player already created");
        else
            s_Instance = new Player(scene);
        return s_Instance;
    }
    
    void Player::ImGuiRender()
    {
        ImGuiAPI::Counter("Player Speed", m_Speed);
        
        if (ImGui::Button("Button"))
        {
            auto& pos = m_Entity.GetComponent<TransformComponent>().Transform[3];
            pos[0] = 0.0f;
            pos[1] = 10.0f;
        }
    }
    
}
