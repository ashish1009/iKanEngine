// ******************************************************************************
//   File    : PlayerController.h
//   Project : Mario
//
//   Created by Ashish
// ******************************************************************************

#pragma once

#include <iKan.h>

#include <Player.h>

namespace Mario {
    
    using namespace iKan;
    
    class PlayerController : public ScriptableEntity
    {
    public:
        virtual void OnUpdate(TimeStep ts) override
        {
            float& xPos = m_Entity.GetComponent<TransformComponent>().Translation.x;
            const float translationSpeed = m_Player.m_TranslationSpeed;
            
            if (Input::IsKeyPressed(KeyCode::Right))
            {
                xPos += translationSpeed;
            }
            if (Input::IsKeyPressed(KeyCode::Left))
            {
                xPos -= translationSpeed;
            }
        }
        
    private:
        Player& m_Player = Player::Get();
    };
    
}
