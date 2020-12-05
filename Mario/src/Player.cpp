#include "Player.h"

#include "NativScripts/PlayerController.h"

namespace Mario {

    Player* Player::s_Instance = nullptr;

    void Player::Init(Ref<Scene>& scene)
    {
        m_Entity = scene->CreateEntity("Player");
        m_Entity.AddComponent<SpriteRendererComponent>();
        m_Entity.AddComponent<NativeScriptComponent>().Bind<PlayerController>();
    }

    void Player::Update(TimeStep ts)
    {
        auto& position = m_Entity.GetComponent<TransformComponent>().Translation;
        float speed = m_Speed * ts;

        if (!(m_Entity.GetScene()->CollisionDetection(m_Entity, speed) & (int)Scene::CollisionSide::Down))
        {
            position.y -= speed;
        }
    }

    void Player::OnKeyPressed(KeyPressedEvent& event)
    {
        if (Key::X == event.GetKeyCode())
        {
        }
    }

    void Player::OnKeyReleased(KeyReleasedEvent& event)
    {

    }

}
