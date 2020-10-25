#include <iKan/Scene/SceneCamera.h>

namespace iKan {
    
    SceneCamera::SceneCamera()
    {
        IK_PROFILE_FUNCTION();

        RecalculateProjection();
    }
    
    void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
    {
        IK_PROFILE_FUNCTION();

        m_ProjectionType   = ProjectionType::Orthographic;
        m_OrthographicSize = size;
        m_OrthographicNear = nearClip;
        m_OrthographicFar  = farClip;
        RecalculateProjection();
    }
    
    void SceneCamera::SetPerspective(float fov, float nearClip, float farClip)
    {
        IK_PROFILE_FUNCTION();

        m_ProjectionType  = ProjectionType::Perspective;
        m_PerspectiveFOV  = fov;
        m_PerspectiveNear = nearClip;
        m_PerspectiveFar  = farClip;
        RecalculateProjection();
    }
    
    void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
    {
        IK_PROFILE_FUNCTION();

        m_AspectRatio = (float)width / (float)height;
        RecalculateProjection();
    }
    
    void SceneCamera::RecalculateProjection()
    {
        IK_PROFILE_FUNCTION();

        if (m_ProjectionType == ProjectionType::Perspective)
        {
            m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
        }
        else if (m_ProjectionType == ProjectionType::Orthographic)
        {
            float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
            float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
            float orthoBottom = -m_OrthographicSize * 0.5f;
            float orthoTop = m_OrthographicSize * 0.5f;
            
            m_Projection = glm::ortho(orthoLeft, orthoRight,
                                            orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
        }
    }
}
