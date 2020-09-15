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
            
            // TODO: argument both Transational speed and fall speed
            Collisions collisionFlags = m_Entity.GetScene()->CollisionDetection(m_Entity, timestep);
            
            player->m_bIsLanded         = (collisionFlags & CollisionBit::Down);
            player->m_bIsRightCollision = (collisionFlags & CollisionBit::Right);
            
            if (Input::IsKeyPressed(Key::Right))
            {
                PlayerRunTexture(timestep);
                // TODO: Add end limit of player move
                if (!player->m_bIsRightCollision)
                    posX += player->m_TranslationSpeed * timestep;
            }
            if(Input::IsKeyPressed(Key::Left))
            {
                PlayerRunTexture(timestep);
                if (posX >= 0 && !player->m_bIsLeftCollision)
                    posX -= player->m_TranslationSpeed * timestep;
            }
            
            if (!player->m_bIsLanded)
            {
                posY -= player->m_LandingSpeed * timestep;
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
