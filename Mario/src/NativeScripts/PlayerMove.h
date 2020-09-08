#pragma once

namespace iKan {
    
    class Player;
    
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
                auto& position = transform[3];
                if(Input::IsKeyPressed(Key::Left))
                    position[0] -= speed * timestep;
                if(Input::IsKeyPressed(Key::Right))
                    position[0] += speed * timestep;
            }
            
            if (Input::IsKeyReleased(Key::Right))
            {
                m_MoveIdx = 0;
                Player::s_Entity.GetComponent<SpriteRendererComponent>().SubTexComp = Player::s_StandSubtexture;
            }
        }
        
    private:
        void PlayerRunTexture(TimeStep timestep)
        {
            auto playerTexture = SubTexture::CreateFromCoords(Player::s_SpriteSheet, { (float)((uint32_t)m_MoveIdx), Player::s_Color });
            Player::s_Entity.GetComponent<SpriteRendererComponent>().SubTexComp = playerTexture;
            m_MoveIdx += (timestep * 15);
            if (m_MoveIdx > 3.0f)
                m_MoveIdx = 0.0f;
        }
        
    private:
        float m_MoveIdx = 0.0f;
        
    };

    
}
