#include <iKan/Editor/EditorCamera.h>
#include <iKan/Core/Input.h>

namespace iKan {
    
    EditorCamera::EditorCamera(float fov, float aspectRatio, float near, float far)
    : m_PerspectiveFOV(fov), m_AspectRatio(aspectRatio), m_PerspectiveNear(near), m_PerspectiveFar(far)
    {
        IK_PROFILE_FUNCTION();

        m_Rotation   = glm::vec3(90.0f, 0.0f, 0.0f);
        m_FocalPoint = glm::vec3(0.0f);
        
        glm::vec3 position = { -15, 5, 5};
        m_Distance = glm::distance(position, m_FocalPoint);
        
        m_Yaw = 3.0f * (float)M_PI / 4.0f;
        m_Pitch = M_PI / 4.0f;
        
        UpdateProjectionMatrix();
        UpdateCameraView();
    }
    
    void EditorCamera::SetViewportSize(uint32_t width, uint32_t height)
    {
        IK_PROFILE_FUNCTION();

        m_ViewportWidth  = width;
        m_ViewportHeight = height;
        m_AspectRatio    = (float)m_ViewportWidth / (float)m_ViewportHeight;
        UpdateProjectionMatrix();
    }
    
    void EditorCamera::UpdateCameraView()
    {
        IK_PROFILE_FUNCTION();

        m_Position = CalculatePosition();
        glm::quat orientation = GetOrientation();
        
        m_Rotation   = glm::eulerAngles(orientation) * (180.0f / (float)M_PI);
        m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
        m_ViewMatrix = glm::inverse(m_ViewMatrix);
    }
    
    std::pair<float, float> EditorCamera::PanSpeed() const
    {
        IK_PROFILE_FUNCTION();

        float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
        float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;
        
        float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
        float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;
        
        return { xFactor, yFactor };
    }

    float EditorCamera::RotationSpeed() const
    {
        IK_PROFILE_FUNCTION();

        return 0.8f;
    }
    
    float EditorCamera::ZoomSpeed() const
    {
        IK_PROFILE_FUNCTION();

        float distance  = m_Distance * 0.2f;
        distance        = std::max(distance, 0.0f);
        
        float speed = distance * distance;
        speed       = std::min(speed, 100.0f);
        
        return speed;
    }
    
    void EditorCamera::OnUpdate(TimeStep ts)
    {
        IK_PROFILE_FUNCTION();

        if (Input::IsKeyPressed(KeyCode::LeftAlt))
        {
            const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
            glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
            m_InitialMousePosition = mouse;
            
            if (Input::IsMouseButtonPressed(MouseCode::ButtonLeft))
                MouseRotate(delta);
        }
        if (Input::IsKeyPressed(KeyCode::LeftControl))
        {
            const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
            glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
            m_InitialMousePosition = mouse;
            
            if (Input::IsMouseButtonPressed(MouseCode::ButtonLeft))
                MousePan(delta);
        }
        if (Input::IsKeyPressed(KeyCode::LeftShift))
        {
            const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
            glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
            m_InitialMousePosition = mouse;
            
            if (Input::IsMouseButtonPressed(MouseCode::ButtonLeft))
                MouseZoom(delta.y);
        }
        
        UpdateCameraView();
    }

    void EditorCamera::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(IK_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
    }
    
    bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
    {
        float delta = e.GetYOffset() * 0.1f;
        MouseZoom(delta);
        UpdateCameraView();
        return false;
    }
    
    void EditorCamera::MousePan(const glm::vec2& delta)
    {
        auto [xSpeed, ySpeed] = PanSpeed();
        m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
        m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
    }
    
    void EditorCamera::MouseRotate(const glm::vec2& delta)
    {
        float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
        m_Yaw   += yawSign * delta.x * RotationSpeed();
        m_Pitch += delta.y * RotationSpeed();
    }
    
    void EditorCamera::MouseZoom(float delta)
    {
        m_Distance -= delta * ZoomSpeed();
        if (m_Distance < 1.0f)
        {
            m_FocalPoint += GetForwardDirection();
            m_Distance = 1.0f;
        }
    }

    glm::vec3 EditorCamera::GetUpDirection() const
    {
        IK_PROFILE_FUNCTION();

        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    
    glm::vec3 EditorCamera::GetRightDirection() const
    {
        IK_PROFILE_FUNCTION();

        return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
    }
    
    glm::vec3 EditorCamera::GetForwardDirection() const
    {
        IK_PROFILE_FUNCTION();

        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
    }

    glm::vec3 EditorCamera::CalculatePosition() const
    {
        IK_PROFILE_FUNCTION();

        return m_FocalPoint - GetForwardDirection() * m_Distance;
    }

    glm::quat EditorCamera::GetOrientation() const
    {
        IK_PROFILE_FUNCTION();

        return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
    }

}
