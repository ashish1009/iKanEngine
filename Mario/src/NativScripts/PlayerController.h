#pragma once

#include <iKan.h>
#include <Player.h>

#define IsCollision(side, speed) (m_Entity.GetScene()->BoxCollisionDetection(m_Entity, speed) & (int)Scene::CollisionSide::side)

using namespace iKan;

namespace Mario {

    class PlayerController : public ScriptableEntity
    {
    public:
        void OnCreate()
        {
        }

        void OnUpdate(TimeStep ts)
        {
            if(Input::IsKeyPressed(Key::Left))
            {
                if (!IsCollision(Left, m_Player.m_TranslationSpeed))
                {
                    m_Player.m_Position.x -= m_Player.m_TranslationSpeed;
                }
            }
            if(Input::IsKeyPressed(Key::Right))
            {
                if (!IsCollision(Right, m_Player.m_TranslationSpeed))
                {
                    m_Player.m_Position.x += m_Player.m_TranslationSpeed;
                }
            }
        }

        void OnCollision(int collision = 0)
        {
            if (collision)
            {
                //
            }
            else
            {
                //
            }
        }

        void OnDestroy()
        {
        }

        // OnAwake(), CollisionCallbacks()

    private:
        Player& m_Player = Player::Get();
    };

}
