#pragma once
#include <Layers/Player.h>

namespace iKan {
    
    class PlayerMove : public ScriptableEntity
    {
    public:

        void OnUpdate(TimeStep timestep)
        {
            auto  player       = Player::Get();
            auto  [posX, posY] = player->GetPosition();
            float speed        = player->m_Speed;
            float fallSpeed    = 15.0f;
            bool  moveFlag     = true;
            
            // TODO: argument both Transational speed and fall speed
            Collisions collisionFlags = m_Entity.GetScene()->CollisionDetection(m_Entity, timestep);
            
            if (collisionFlags & CollisionBit::Down)
            {
                player->m_bIsLanded = true;
            }
            else
            {
                player->m_bIsLanded = false;
            }
            if (collisionFlags & CollisionBit::Right)
            {
                moveFlag = false;
            }
            else
            {
                moveFlag = true;
            }
            
            if (Input::IsKeyPressed(Key::Right))
            {
                PlayerRunTexture(timestep);
                // TODO: Add end limit of player move
                if (moveFlag)
                    posX += speed * timestep;
            }
            if(Input::IsKeyPressed(Key::Left))
            {
                PlayerRunTexture(timestep);
                if (posX >= 0)
//                    if (moveFlag)
                        posX -= speed * timestep;
            }
            
            if (!player->m_bIsLanded)
            {
//                IK_CORE_INFO("{0}", posY);
//                posY -= fallSpeed * timestep;
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
