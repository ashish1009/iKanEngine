#include "Player.h"

namespace iKan {

    Entity Player::s_Entity;
    float Player::s_Color = static_cast<float>(PlayerColor::BlueOrange);
    std::shared_ptr<SubTexture> Player::s_StandSubtexture = nullptr;
    std::shared_ptr<Texture> Player::s_SpriteSheet = nullptr;
    
    void Player::Init(const std::shared_ptr<Scene>& scene)
    {
        IK_INFO("Player Instance Created");
        
        s_SpriteSheet     = Texture::Create("../../Mario/assets/Resources/Graphics/Player.png");
        s_StandSubtexture = SubTexture::CreateFromCoords(s_SpriteSheet, { 6.0f, s_Color });
                
        s_Entity = scene->CreateEntity("Player");
        s_Entity.AddComponent<SpriteRendererComponent>(s_StandSubtexture);
        
        class PlayerMove : public ScriptableEntity
        {
        public:
            void OnUpdate(TimeStep timestep)
            {
                if (Input::IsKeyPressed(Key::Right))
                {
                    PlayerRunTexture(timestep);
                    float speed = 10;
                    
                    auto& transform = GetComponent<TransformComponent>().Transform;
                    auto position = transform[3];
                    if(Input::IsKeyPressed(Key::Left))
                        position[0] -= speed * timestep;
                    if(Input::IsKeyPressed(Key::Right))
                        position[0] += speed * timestep;
                }
                
                if (Input::IsKeyReleased(Key::Right))
                {
                    m_MoveIdx = 0;
                    Player::s_Entity.GetComponent<SpriteRendererComponent>().SubTexComp = Player::s_StandSubtexture;;
                }
            }
            
        private:
            void PlayerRunTexture(TimeStep timestep)
            {
                auto playerTexture = SubTexture::CreateFromCoords(s_SpriteSheet, { (float)((uint32_t)m_MoveIdx), s_Color });
                Player::s_Entity.GetComponent<SpriteRendererComponent>().SubTexComp = playerTexture;
                m_MoveIdx += (timestep * 15);
                if (m_MoveIdx > 3.0f)
                    m_MoveIdx = 0.0f;
            }
            
        private:
            float m_MoveIdx = 0.0f;

        };
        
        s_Entity.AddComponent<NativeScriptComponent>().Bind<PlayerMove>();
    }
        
}
