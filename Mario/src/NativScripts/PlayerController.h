#pragma once

#include <iKan.h>
#include <Player.h>

#include <BackgroundTiles.h>

#define IsCollision(side, speed) (m_Entity.GetScene()->BoxCollisionDetection(m_Entity, speed) & (int)Scene::CollisionSide::side)

using namespace iKan;

namespace Mario {

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

        void OnCollision(Entity* colloidedEntity, int32_t collision = 0)
        {
            if (collision)
            {
                CollisionCallback(Scene::CollisionSide::Up, collision, colloidedEntity[GetBitPos((int32_t)Scene::CollisionSide::Up)]);
                CollisionCallback(Scene::CollisionSide::Down, collision, colloidedEntity[GetBitPos((int32_t)Scene::CollisionSide::Down)]);
                CollisionCallback(Scene::CollisionSide::Left, collision, colloidedEntity[GetBitPos((int32_t)Scene::CollisionSide::Left)]);
                CollisionCallback(Scene::CollisionSide::Right, collision, colloidedEntity[GetBitPos((int32_t)Scene::CollisionSide::Right)]);
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

        void CollisionCallback(Scene::CollisionSide collisionSide, int32_t collision, Entity colloidedEntity)
        {
            int32_t index = GetBitPos(collision & (int32_t)collisionSide);
            if (index >= 0)
                (this->*m_CollisionFnPtr[index])(colloidedEntity);
        }

        void UpCollision(Entity colloidedEntity)
        {
            const auto& colloidedEntPos = colloidedEntity.GetComponent<TransformComponent>().Translation;
            auto& playerPos = m_Player.m_Position;

            if ((playerPos.x - colloidedEntPos.x) > 0.5f && (playerPos.x - colloidedEntPos.x) < 0.9f)
            {
                playerPos.x = std::floor(playerPos.x) + 1.0f;
                m_Player.m_InAirOffset = true;
            }

            else if ((playerPos.x - colloidedEntPos.x) < -0.5f && (playerPos.x - colloidedEntPos.x) > -0.9f)
            {
                playerPos.x = std::floor(playerPos.x);
                m_Player.m_InAirOffset = true;
            }

            else
            {
                m_Player.m_InAirOffset = false;
                // TODO: make something more clean
                if (colloidedEntity.GetComponent<TagComponent>().Tag == "Bricks")
                {
                    colloidedEntity.GetScene()->DestroyEntity(colloidedEntity);
                }
                else if (colloidedEntity.GetComponent<TagComponent>().Tag == "Bonus")
                {
                    BackgroundTile::ChangeBonusTextureToUsed(colloidedEntity);
                }
                else
                {

                }
            }
        }

        void DownCollision(Entity colloidedEntity)
        {

        }

        void RightCollision(Entity colloidedEntity)
        {

        }

        void LeftCollision(Entity colloidedEntity)
        {

        }

    private:
        Player& m_Player = Player::Get();

        std::array<void (PlayerController::*)(Entity colloidedEntity), 4> m_CollisionFnPtr;
    };

}
