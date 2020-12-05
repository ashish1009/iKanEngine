#pragma once

#include <iKan.h>

using namespace iKan;

namespace Mario {

    class Player
    {
    public:
        static constexpr float TranslationSpeed = 12.5;
        static constexpr float JumpSpeed        = 12.5;
        static constexpr float FreeFallSpeed    = 12.5;

    public:
        enum class State { Falling = 0, Standing, Jumping, LAST };

    public:
        Player(const Player& other) = delete;
        Player(Player&& other)      = delete;

        Player& operator =(const Player& other) = delete;
        Player& operator =(Player&& other)      = delete;

        static Player& Get()
        {
            if (!s_Instance)
                s_Instance = new Player();
            return *s_Instance;
        }

        void Init(Ref<Scene>& scene);
        void Update(TimeStep ts);
        void OnKeyPressed(KeyPressedEvent& event);
        void OnKeyReleased(KeyReleasedEvent& event);

        void SetTranslationSpeed(float speed) { m_TranslationSpeed = speed; }
        void SetFreeFallSpeed(float speed) { m_FreeFallSpeed = speed; }
        void SetJumpSpeed(float speed) { m_JumpSpeed = speed; }
        void SetState(State state) { m_State = state; }

        float GetTranslationSpeed() const { return m_TranslationSpeed; }
        float GetFreeFallSpeed() const { return m_FreeFallSpeed; }
        float GetJumpSpeed() const { return m_JumpSpeed; }
        State GetState() const { return m_State; }

        void FreeFall();
        void Standing();
        void Jumping();

    private:
        Player() = default;
        ~Player() = default;

    private:
        static Player* s_Instance;

        std::array<void (Player::*)(), size_t(State::LAST)> m_PlayerUpdateFnPtr;

        State    m_State            = State::Falling;
        float    m_TranslationSpeed = 1.0f;
        float    m_FreeFallSpeed    = 1.0f;
        float    m_JumpSpeed        = 1.0f;

        Entity   m_Entity;
    };

}
