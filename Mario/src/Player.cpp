#include "Player.h"

#include "NativScripts/PlayerController.h"

namespace Mario {

// Clear a state from current states
#define ClearState(state)           m_State = (m_State & (~(1 << (int32_t)log2((int32_t)State::state))))

// Adds a state into current states
#define AddState(state)             m_State = (m_State | (int32_t)State::state)

// Set only one state
#define SetState(state)             m_State = 0; m_State = (int32_t)State::state;

    Player* Player::s_Instance = nullptr;

    static int32_t GetBitPos(int32_t state)
    {
        if (!state)
            return -1;
        return log2(state);
    }

    void Player::StateCallback(State state)
    {
        int32_t index = GetBitPos(m_State & (int32_t)state);
        if (index >= 0)
            (this->*m_PlayerStateFnPtr[index])();
    }

    void Player::Init(Ref<Scene>& scene)
    {
        IK_INFO("Player Instance Created");

        m_SpriteSheet = Texture::Create("../../Mario/assets/Resources/Graphics/Player.png");

        for (int32_t i = 0; i < MaxPlayerImages; i++)
        {
            m_StandSubtexture[i] = SubTexture::CreateFromCoords(m_SpriteSheet, { 6.0f, float(i * PlayerImgColorDiff) });

            for (int32_t j = 0; j < PlayerRunImages; j++)
            {
                m_RunningSubtexture[i][j] = SubTexture::CreateFromCoords(m_SpriteSheet, { float(j), float(i * PlayerImgColorDiff) });
            }
        }

        m_Entity = scene->CreateEntity("Player");
        m_Entity.AddComponent<SpriteRendererComponent>();
        m_Entity.AddComponent<NativeScriptComponent>().Bind<PlayerController>();

        m_Position = m_Entity.GetComponent<TransformComponent>().Translation;

        // Registering the callback function according to the states
        m_PlayerStateFnPtr[GetBitPos((int32_t)State::Standing)] = &Player::Stand;
        m_PlayerStateFnPtr[GetBitPos((int32_t)State::Falling)]  = &Player::Freefall;
        m_PlayerStateFnPtr[GetBitPos((int32_t)State::Jumping)]  = &Player::Jump;
    }

    void Player::Update(TimeStep ts)
    {
        m_TranslationSpeed = TranslationSpeed * ts;
        m_FreeFallSpeed    = FreeFallSpeed * ts;
        m_JumpSpeed        = JumpSpeed * ts;

        {
            if (IsCollision(Down, m_FreeFallSpeed))
            {
                ClearState(Falling);
                AddState(Standing);
            }
            else
            {
                if (!(m_State & (int32_t)State::Jumping))
                {
                    ClearState(Standing);
                    AddState(Falling);
                }
            }
        }

        // Change the Entity size so that it can revert its image with direction change
        auto& scale = m_Entity.GetComponent<TransformComponent>().Scale;
        if (m_Direction == Right)
        {
            scale.x = 1.0f;
        }
        else if (m_Direction == Left)
        {
            scale.x = -1.0f;
        }
        else
        {
            IK_ERROR("Invalid Direction");
        }

        StateCallback(State::Standing);
        StateCallback(State::Falling);
        StateCallback(State::Jumping);

        // Update the local position to the entity
        m_Entity.GetComponent<TransformComponent>().Translation = m_Position;
    }

    void Player::UpdateRunningImage(int32_t runImgIdx)
    {
        m_Entity.GetComponent<SpriteRendererComponent>().SubTexComp = m_RunningSubtexture[int32_t(m_Color / PlayerImgColorDiff)][m_RunningImgIdx];

        m_RunningImgIdx = runImgIdx;
        m_RunningImgIdx = m_RunningImgIdx % 3;
    }

    void Player::Stand()
    {
        m_Position.y = std::floor(m_Position.y);
        m_Entity.GetComponent<SpriteRendererComponent>().SubTexComp = m_StandSubtexture[int32_t(m_Color / PlayerImgColorDiff)];
    }

    void Player::Freefall()
    {
        m_Position.y -= m_FreeFallSpeed;
    }

    void Player::Jump()
    {

    }

    void Player::OnKeyPressed(KeyPressedEvent& event)
    {
        if (Key::X == event.GetKeyCode())
        {
        }
    }

    void Player::OnKeyReleased(KeyReleasedEvent& event)
    {
        if (Key::Right == event.GetKeyCode() || Key::Left == event.GetKeyCode())
        {
            m_RunningImgIdx = 0;
        }
    }

    void Player::ImGuiRenderer()
    {
        if (ImGui::TreeNode("Player..."))
        {
            ImTextureID myTexId = (ImTextureID)((size_t)m_SpriteSheet->GetRendererID());
            float myTexW = (float)m_SpriteSheet->GetWidth();
            float myTexH = (float)m_SpriteSheet->GetHeight();

            // there are 10 Images for player
            for (int32_t i = 0; i < MaxPlayerImages; i++)
            {
                ImGui::PushID(i);

                float X = 6.0f;
                float Y = float(i * PlayerImgColorDiff);

                glm::vec2 uv1 = { (X + 1) * 16.0f, Y * 16.0f };
                glm::vec2 uv0 = { X * 16.0f, (Y + 1) * 16.0f };

                if (ImGui::ImageButton(myTexId, ImVec2(32.0f, 32.0f), ImVec2(uv0.x / myTexW, uv0.y / myTexH), ImVec2(uv1.x / myTexW, uv1.y / myTexH), 0))
                {
                    if (auto &subTexComp = m_Entity.GetComponent<SpriteRendererComponent>().SubTexComp)
                    {
                        m_Color = float(i * PlayerImgColorDiff);
                    }
                }
                ImGui::PopID();
                ImGui::SameLine();
            }

            ImGui::NewLine();
            ImGui::TreePop();
        }
    }
}
