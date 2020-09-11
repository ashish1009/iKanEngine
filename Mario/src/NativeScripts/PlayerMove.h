#pragma once

namespace iKan {
    
    class Player;
    
    class PlayerMove : public ScriptableEntity
    {
    public:
        void OnUpdate(TimeStep timestep)
        {
            auto& position = GetComponent<TransformComponent>().Transform[3];
            float speed = 10;
            
            if (Input::IsKeyPressed(Key::Right))
            {
                PlayerRunTexture(timestep);
                position[0] += speed * timestep;
            }
            if(Input::IsKeyPressed(Key::Left))
            {
                PlayerRunTexture(timestep);
                position[0] -= speed * timestep;
            }
        }
        
    private:
        void PlayerRunTexture(TimeStep timestep)
        {
            auto player = Player::Get();
            auto playerTexture = SubTexture::CreateFromCoords(player->m_SpriteSheet, { (float)((uint32_t)player->m_MoveIdx), player->m_Color });
            player->m_Entity.GetComponent<SpriteRendererComponent>().SubTexComp = playerTexture;
            player->m_MoveIdx += (timestep * 15);
            if (player->m_MoveIdx > 3.0f)
                player->m_MoveIdx = 0.0f;
        }
                
    };

    
}
