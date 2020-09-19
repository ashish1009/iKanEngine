#include <Layers/Player.h>

#include <NativeScripts/PlayerMove.h>

namespace iKan {

    Player* Player::s_Instance = nullptr;
    
    static std::vector<std::shared_ptr<SubTexture>> s_PlayerSubTextures;
    
    Player::Player(const std::shared_ptr<Scene>& scene)
    {
        IK_INFO("Player Instance Created");

        m_SpriteSheet     = Texture::Create("../../Mario/assets/Resources/Graphics/Player.png");
        
        m_StandSubtexture = SubTexture::CreateFromCoords(m_SpriteSheet, { 6.0f, m_ColorIndex }, {1.0f, 2.0f});
        
        // TODO: there are 14 textures for small player and 20 for Big one
        // Do them Dynamyc later
        for (size_t i = 0; i < 14; i++)
            s_PlayerSubTextures.emplace_back(SubTexture::CreateFromCoords(m_SpriteSheet, { i, m_ColorIndex }));

        m_Entity = scene->CreateEntity("Player");
        
        m_Entity.GetComponent<TransformComponent>().Transform[3][1] = 10.0f;
        m_Entity.GetComponent<TransformComponent>().Transform[3][0] = 3.0f;
        
        //TODO: Temp
        m_Entity.GetComponent<TransformComponent>().SetTransform({m_Entity.GetComponent<TransformComponent>().Transform[3][0], m_Entity.GetComponent<TransformComponent>().Transform[3][1] }, { 1.0f, 2.0f });
        
        m_Entity.AddComponent<SpriteRendererComponent>(m_StandSubtexture);
        
        m_Entity.AddComponent<NativeScriptComponent>();
        m_Entity.GetComponent<NativeScriptComponent>().Bind<PlayerMove>();
    }
    
    bool Player::OnKeyPressed(KeyPressedEvent& event)
    {
//        if (event.GetKeyCode() == Key::Right)
//        {
//            float& posX = GetPositionX();
//
//            PlayerRunTexture(timestep);
//             TODO: Add end limit of player move
//            IK_CORE_INFO("{0}", event.GetKeyRepeat());
//
//            auto playerTexture = SubTexture::CreateFromCoords(m_SpriteSheet, { (float)((uint32_t)m_MoveIdx), m_ColorIndex }, {1.0f, 2.0f});
//            m_Entity.GetComponent<SpriteRendererComponent>().SubTexComp = playerTexture;
//            m_MoveIdx ++;//= (timestep * 15);
//            if (m_MoveIdx > 2.0f)
//                m_MoveIdx = 0.0f;
//
//
//            if (!m_bIsRightCollision)
//            {
//                posX += ((1.0 + event.GetKeyRepeat()) / 16.0f);
//            }
//        }
//        if(event.GetKeyCode() == Key::Left)
//        {
//            float& posX = GetPositionX();
//
//            PlayerRunTexture(timestep);
//            if (posX >= 0 && !m_bIsLeftCollision)
//                posX -= (1.0f / 16.0f);
//        }
//
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
