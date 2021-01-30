#pragma once

#include <iKan.h>
#include <Player.h>

#define IsCollision(side, speed) (m_Entity.GetScene()->BoxCollisionDetection(m_Entity, speed) & (int)Scene::CollisionSide::side)

using namespace iKan;

namespace Mario {

    static int32_t GetBitPos(int32_t state)
    {
        if (!state)
            return -1;
        return log2(state);
    }

    class PlayerController : public ScriptableEntity
    {
    public:
        void OnCreate()
        {
            m_CollisionFnPtr[GetBitPos((int32_t)Scene::CollisionSide::Up)]    = &PlayerController::UpCollision;
            m_CollisionFnPtr[GetBitPos((int32_t)Scene::CollisionSide::Down)]  = &PlayerController::DownCollision;
            m_CollisionFnPtr[GetBitPos((int32_t)Scene::CollisionSide::Right)] = &PlayerController::RightCollision;
            m_CollisionFnPtr[GetBitPos((int32_t)Scene::CollisionSide::Left)]  = &PlayerController::LeftCollision;
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

                    if (cameraPosition.x >= s_CommonStartPos)
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

                    if (playerPosition.x - cameraPosition.x >= 0)
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
                CollisionCallback(Scene::CollisionSide::Up, collision);
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

        void CollisionCallback(Scene::CollisionSide collisionSide, int collision)
        {
            int32_t index = GetBitPos(collision & (int32_t)collisionSide);
            if (index >= 0)
                (this->*m_CollisionFnPtr[index])();
        }

        void UpCollision()
        {

        }

        void DownCollision()
        {

        }

        void RightCollision()
        {

        }

        void LeftCollision()
        {

        }

    private:
        Player& m_Player = Player::Get();

        std::array<void (PlayerController::*)(), 4> m_CollisionFnPtr;
    };

}
