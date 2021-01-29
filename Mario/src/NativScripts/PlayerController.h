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
            static float i = 0;
            if(Input::IsKeyPressed(Key::Left))
            {
                if (!IsCollision(Left, m_Player.m_TranslationSpeed) && m_Player.m_Position.x >= 0)
                {
                    auto& cameraPosition = m_Player.m_CameraEntity.GetComponent<TransformComponent>().Translation;
                    auto& playerPosition = m_Player.m_Position;

                    // Change the Player direction
                    m_Player.m_Direction = Direction::Left;

                    // Chnage the position of Player and camera
                    playerPosition.x -= m_Player.m_TranslationSpeed;
                    cameraPosition.x -= m_Player.m_TranslationSpeed;

                    // Change the image of player while running
                    m_Player.UpdateRunningImage(int32_t(i));
                }
            }
            if(Input::IsKeyPressed(Key::Right))
            {
                if (!IsCollision(Right, m_Player.m_TranslationSpeed))
                {
                    auto& cameraPosition = m_Player.m_CameraEntity.GetComponent<TransformComponent>().Translation;
                    auto& playerPosition = m_Player.m_Position;

                    // Change the Player direction
                    m_Player.m_Direction = Direction::Right;

                    // Chnage the position of Player and camera
                    playerPosition.x += m_Player.m_TranslationSpeed;
                    cameraPosition.x += m_Player.m_TranslationSpeed;

                    // Change the image of player while running
                    m_Player.UpdateRunningImage(int32_t(i));
                }
            }

            // Updating the counter Ts multiple of 10 so that ints integrav value is 100ms rather than 1 sec (1000ms)
            i += ts * 10;
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
