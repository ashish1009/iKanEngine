#pragma once

#include <iKan.h>

using namespace iKan;

namespace Mario {

    class PlayerController;

    static const int32_t MaxPlayerImages    = 10;
    static const int32_t PlayerImgColorDiff = 3;
    static const int32_t PlayerRunImages    = 3;

    enum class State
    {
        Standing = BIT(0),
        Falling  = BIT(1),
        Jumping  = BIT(2),
    };

    enum Direction
    {
        Left = 0, Right = 1
    };

    class Player
    {
    public:
        // For Big Player Add the value by 1
        enum class PlayerColor
        {
            Black = 0, Skin = 3, Grey = 6, Blue = 9, BlueOrange = 12, RedOrange = 15, GreenOrange = 18, BlackOrange = 21, GreeWhite = 24, RedWhite = 27, Basic = 30,
        };

    public:
        static constexpr float TranslationSpeed = 6;
        static constexpr float JumpSpeed        = 12.5;
        static constexpr float FreeFallSpeed    = 25;

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

        float GetTranslationSpeed() const { return m_TranslationSpeed; }
        float GetFreeFallSpeed() const { return m_FreeFallSpeed; }
        float GetJumpSpeed() const { return m_JumpSpeed; }

        glm::vec3& GetPosition() { return m_Position; }

        void ImGuiRenderer();

        void UpdateRunningImage(int32_t runImgIdx);

    private:
        Player() = default;
        ~Player() = default;

        void Stand();
        void Freefall();
        void Jump();

        void StateCallback(State state);

    private:
        static Player* s_Instance;
        Entity m_Entity;

        // 3 : Using 3 as there are 3 states available
        std::array<void (Player::*)(), 3> m_PlayerStateFnPtr;

        glm::vec3 m_Position;

        // Speed Variables
        float m_TranslationSpeed;
        float m_FreeFallSpeed;
        float m_JumpSpeed;

        // Color of player
        float m_Color = (float)(PlayerColor::RedOrange);

        // Storing State in int as it can store multiple state at one time
        int32_t m_State = (int32_t)(State::Falling);

        // Diretion of player
        Direction m_Direction = Direction::Right;

        // Subtexture of player stored as per its state
        Ref<SubTexture> m_StandSubtexture[MaxPlayerImages];
        Ref<SubTexture> m_RunningSubtexture[MaxPlayerImages][PlayerRunImages];
        
        Ref<Texture>    m_SpriteSheet;

        // Index of player image while running
        int32_t m_RunningImgIdx = 0;

        friend class PlayerController;
    };

}
