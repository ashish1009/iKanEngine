#include "Player.h"

#include "NativScripts/PlayerController.h"

namespace Mario {

#define ToggleState(state)          m_State = (m_State ^ (1 << (int32_t)log2((int32_t)State::state)))
#define ClearState(state)           m_State = (m_State & (~(1 << (int32_t)log2((int32_t)State::state))))
#define SetState(state)             m_State = (m_State | (int32_t)State::state)
#define IsOnlyStateEnabled(state)   !(m_State & (~(1 << (int32_t)log2((int32_t)State::state))))

    static float jumpHeight;

    Player* Player::s_Instance = nullptr;

    static size_t GetBitPos(int32_t state)
    {
        if (!state)
            return 0;
        return log2(state);
    }

    void Player::Init(Ref<Scene>& scene)
    {
        m_Entity = scene->CreateEntity("Player");
        m_Entity.AddComponent<SpriteRendererComponent>();
        m_Entity.AddComponent<NativeScriptComponent>().Bind<PlayerController>();

        m_Position = m_Entity.GetComponent<TransformComponent>().Translation;

        // Registering the callback function according to the states
        m_PlayerUpdateFnPtr[GetBitPos((int32_t)State::Standing)] = &Player::Stand;
        m_PlayerUpdateFnPtr[GetBitPos((int32_t)State::Falling)] = &Player::FreeFall;
        m_PlayerUpdateFnPtr[GetBitPos((int32_t)State::Jumping)] = &Player::Jump;
    }

    void Player::StateCallbacks(State state)
    {
        size_t index = GetBitPos(m_State & (int32_t)state);
        if (index)
            (this->*m_PlayerUpdateFnPtr[index])();
    }

    void Player::Update(TimeStep ts)
    {
        m_TranslationSpeed = TranslationSpeed * ts;
        m_FreeFallSpeed    = FreeFallSpeed * ts;
        m_JumpSpeed        = JumpSpeed * ts;

        // Update the states
        {
            if (IsCollision(Down, m_FreeFallSpeed))
            {
                ClearState(Falling);
                SetState(Standing);
                jumpHeight = m_Position.y;
            }
            else
            {
                if (!(m_State & (int32_t)State::Jumping))
                {
                    SetState(Falling);
                }
            }
        }

        StateCallbacks(State::Standing);
        StateCallbacks(State::Falling);
        StateCallbacks(State::Jumping);

        // Update the local position to the entity
        m_Entity.GetComponent<TransformComponent>().Translation = m_Position;
    }

    void Player::OnKeyPressed(KeyPressedEvent& event)
    {
        if (Key::X == event.GetKeyCode())
        {
            if (IsOnlyStateEnabled(Standing))
            {
                SetState(Jumping);
                ClearState(Falling);
            }
        }
    }

    void Player::OnKeyReleased(KeyReleasedEvent& event)
    {

    }

    void Player::FreeFall()
    {
        m_Position.y -= m_FreeFallSpeed;
    }

    void Player::Stand()
    {

    }

    void Player::Jump()
    {
        if ((m_Position.y - jumpHeight < 5) && (!IsCollision(Up, m_JumpSpeed)))
        {
            m_Position.y += m_JumpSpeed;
        }
        else
        {
            ClearState(Jumping);
        }
    }

}
