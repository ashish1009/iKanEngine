#include "Player.h"

#include "NativScripts/PlayerController.h"

namespace Mario {

    Player* Player::s_Instance = nullptr;

    void Player::Init(Ref<Scene>& scene)
    {
        m_Entity = scene->CreateEntity("Player");
        m_Entity.AddComponent<SpriteRendererComponent>();
        m_Entity.AddComponent<NativeScriptComponent>().Bind<PlayerController>();

        // Registering the callback function according to the states
        m_PlayerUpdateFnPtr[int(State::Falling)]  = &Player::FreeFall;
        m_PlayerUpdateFnPtr[int(State::Standing)] = &Player::Standing;
        m_PlayerUpdateFnPtr[int(State::Jumping)]  = &Player::Jumping;
    }

    void Player::Update(TimeStep ts)
    {
        m_TranslationSpeed = TranslationSpeed * ts;
        m_State = State::Falling;

        // Update the states
        {
            if (m_Entity.GetScene()->CollisionDetection(m_Entity, m_TranslationSpeed) & (int)Scene::CollisionSide::Down)
                m_State = State::Standing;
        }

        IK_CORE_ASSERT((m_State < State::LAST), "Invalid State");
        (this->*m_PlayerUpdateFnPtr[int(m_State)])();
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

    void Player::FreeFall()
    {
        auto& position = m_Entity.GetComponent<TransformComponent>().Translation;
        position.y -= m_TranslationSpeed;
    }

    void Player::Standing()
    {

    }

    void Player::Jumping()
    {

    }

}
