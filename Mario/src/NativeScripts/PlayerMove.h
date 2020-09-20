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
            
            float translationSpeed = player->m_TranslationSpeed * timestep;
            float landingSpeed = player->m_LandingSpeed * timestep;
            float jumpingSpeed = player->m_JumpingSpeed * timestep;
            
            // TODO: argument both Transational speed and fall speed
            Collisions collisionFlags = m_Entity.GetScene()->CollisionDetection(m_Entity, Scene::Speeds(jumpingSpeed, landingSpeed, translationSpeed, translationSpeed));
            
            player->m_bIsLanded         = (collisionFlags & CollisionBit::Down);
            player->m_bIsRightCollision = (collisionFlags & CollisionBit::Right);
            player->m_bIsLeftCollision  = (collisionFlags & CollisionBit::Left);
            
            if (!player->m_bIsLanded)
                posY -= landingSpeed;

            if (Input::IsKeyPressed(Key::Right))
            {
                PlayerRunTexture(timestep);
                // TODO: Add end limit of player move
                if (!player->m_bIsRightCollision)
                    posX += translationSpeed;
            }
            if(Input::IsKeyPressed(Key::Left))
            {
                PlayerRunTexture(timestep);
                if (posX >= 0 && !player->m_bIsLeftCollision)
                    posX -= translationSpeed;
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
