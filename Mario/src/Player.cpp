#include "Player.h"

#include "NativScripts/PlayerController.h"

namespace Mario {

// Clear a state from current states
#define ClearState(state)           m_State = (m_State & (~(1 << (int32_t)log2((int32_t)State::state))))

// Adds a state into current states
#define AddState(state)             m_State = (m_State | (int32_t)State::state)

// Set only one state
#define SetState(state)             m_State = 0; m_State = (int32_t)State::state;

// Check if one state is present or not
#define IsState(state)              (m_State & (int32_t)State::state)

    Player* Player::s_Instance = nullptr;

    void Player::StateCallback(State state)
    {
        int32_t index = GetBitPos(m_State & (int32_t)state);
        if (index >= 0)
            (this->*m_PlayerStateFnPtr[index])();
    }

    void Player::Init(Ref<Scene>& scene, Entity cameraEntity)
    {
        IK_INFO("Player Instance Created");
        m_CameraEntity = cameraEntity;
        m_SpriteSheet = Texture::Create("../../Mario/assets/Resources/Graphics/Player.png");

        // Storing all kind of images in init time
        for (int32_t i = 0; i < MaxPlayerImages; i++)
        {
            m_StandSubtexture[i] = SubTexture::CreateFromCoords(m_SpriteSheet, { 6.0f, float(i * PlayerImgColorDiff) + 1.0f }, { 1.0f, 2.0f });
            m_JumpSubtexture[i]  = SubTexture::CreateFromCoords(m_SpriteSheet, { 4.0f, float(i * PlayerImgColorDiff) });

            for (int32_t j = 0; j < PlayerRunImages; j++)
            {
                m_RunningSubtexture[i][j] = SubTexture::CreateFromCoords(m_SpriteSheet, { float(j), float(i * PlayerImgColorDiff) });
            }
        }

        // Creating Player Entity
        m_Entity = scene->CreateEntity("Player");
        m_Entity.AddComponent<SpriteRendererComponent>();
        m_Entity.AddComponent<NativeScriptComponent>().Bind<PlayerController>();

        // Bring player in front
        m_Entity.GetComponent<TransformComponent>().Translation.x = s_CommonStartPos;

        m_Position = m_Entity.GetComponent<TransformComponent>().Translation;

        // Registering the callback function according to the states
        m_PlayerStateFnPtr[GetBitPos((int32_t)State::Standing)] = &Player::Stand;
        m_PlayerStateFnPtr[GetBitPos((int32_t)State::Falling)]  = &Player::Freefall;
        m_PlayerStateFnPtr[GetBitPos((int32_t)State::Jumping)]  = &Player::Jump;

        ChangeSize(Size::Large);
    }

    void Player::ChangeSize(Size size)
    {
        m_Size = size;

        auto& scale = m_Entity.GetComponent<TransformComponent>().Scale.y;

        float tileSize;
        if (Size::Large == m_Size)
        {
            tileSize = 2.0f;
            scale = 2.0f;
        }
        else if (Size::Small == m_Size)
        {
            tileSize = 1.0f;
            scale = 1.0f;
        }
        else
        {
            tileSize = 0.0f;
            IK_ASSERT(false, "Invalid Size");
        }

        for (int32_t i = 0; i < MaxPlayerImages; i++)
        {
            m_StandSubtexture[i] = SubTexture::CreateFromCoords(m_SpriteSheet, { 6.0f, float(i * PlayerImgColorDiff) + 1.0f }, { 1.0f, tileSize });
            m_JumpSubtexture[i]  = SubTexture::CreateFromCoords(m_SpriteSheet, { 4.0f, float(i * PlayerImgColorDiff) + 1.0f }, { 1.0f, tileSize });

            for (int32_t j = 0; j < PlayerRunImages; j++)
            {
                m_RunningSubtexture[i][j] = SubTexture::CreateFromCoords(m_SpriteSheet, { float(j), float(i * PlayerImgColorDiff) + 1.0f }, { 1.0f,tileSize });
            }
        }

    }

    void Player::Update(TimeStep ts)
    {
        m_TranslationSpeed = TranslationSpeed * ts;
        m_FreeFallSpeed    = FreeFallSpeed * ts;
        m_JumpSpeed        = JumpSpeed * ts;

        // Landing Logic and states
        {
            if (!IsState(Jumping) && IsCollision(Down, m_FreeFallSpeed))
            {
                m_Entity.GetComponent<SpriteRendererComponent>().SubTexComp = m_StandSubtexture[int32_t(m_Color)];

                ClearState(Falling);
                AddState(Standing);
            }
            else
            {
                if (!IsState(Jumping))
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

        // State Callbacks
        StateCallback(State::Standing);
        StateCallback(State::Falling);
        StateCallback(State::Jumping);

        // Update the local position to the entity
        m_Entity.GetComponent<TransformComponent>().Translation = m_Position;
    }

    void Player::UpdateRunningImage(int32_t runImgIdx)
    {
        if (IsState(Standing))
        {
            m_Entity.GetComponent<SpriteRendererComponent>().SubTexComp = m_RunningSubtexture[int32_t(m_Color)][m_RunningImgIdx];

            m_RunningImgIdx = runImgIdx;
            m_RunningImgIdx = m_RunningImgIdx % 3;
        }
    }

    void Player::Stand()
    {
        if (Size::Small == m_Size)
            m_Position.y = std::floor(m_Position.y);
        else if (Size::Large == m_Size)
            m_Position.y = std::floor(m_Position.y) + 0.5f;
    }

    void Player::Freefall()
    {
        m_Position.y -= m_FreeFallSpeed;
    }

    void Player::Jump()
    {
        m_Entity.GetComponent<SpriteRendererComponent>().SubTexComp = m_JumpSubtexture[int32_t(m_Color)];

        if (m_Position.y - m_JumpBeginPosition > MaxJumpHeight)
        {
            ClearState(Jumping);
            AddState(Falling);
        }
        else
        {
            if (!IsCollision(Up, m_JumpSpeed))
            {
                m_Position.y += m_JumpSpeed;
            }
            else
            {
                if (m_InAirOffset)
                {

                }
                else
                {
                    ClearState(Jumping);
                    AddState(Falling);
                }
            }
        }
    }

    void Player::OnKeyPressed(KeyPressedEvent& event)
    {
        if ((Key::X == event.GetKeyCode()) && (IsState(Standing)))
        {
            ClearState(Falling);
            ClearState(Standing);
            AddState(Jumping);

            m_JumpBeginPosition = m_Position.y;
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
                float Y = float(i);

                glm::vec2 uv1 = { (X + 1) * 16.0f, Y * 16.0f };
                glm::vec2 uv0 = { X * 16.0f, (Y + 1) * 16.0f };

                if (ImGui::ImageButton(myTexId, ImVec2(32.0f, 32.0f), ImVec2(uv0.x / myTexW, uv0.y / myTexH), ImVec2(uv1.x / myTexW, uv1.y / myTexH), 0))
                {
                    if (auto &subTexComp = m_Entity.GetComponent<SpriteRendererComponent>().SubTexComp)
                    {
                        m_Color = float(i);
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
