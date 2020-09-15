#pragma once
#include <Layers/Player.h>

namespace iKan {
    
    class PlayerMove : public ScriptableEntity
    {
    public:

        void OnUpdate(TimeStep timestep)
        {
            auto player       = Player::Get();
            auto [posX, posY] = player->GetPosition();
            float speed       = player->m_Speed;
            float fallSpeed   = 15.0f;
            
            if (m_Entity.GetScene()->CollisionDetection(m_Entity, fallSpeed * timestep))
            {
                player->m_bIsLanded = true;
            }
            else
            {
                player->m_bIsLanded = false;
            }
            
            if (Input::IsKeyPressed(Key::Right))
            {
                PlayerRunTexture(timestep);
                // TODO: Add end limit of player move
                posX += speed * timestep;
            }
            if(Input::IsKeyPressed(Key::Left))
            {
                PlayerRunTexture(timestep);
                if (posX >= 0)
                    posX -= speed * timestep;
            }
            
            if (!player->m_bIsLanded)
            {
                posY -= fallSpeed * timestep;
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
