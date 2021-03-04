// ******************************************************************************
//   File    : SceneCamera.h
//   Project : i-Kan : Scene
//
//   Created by Ashish
// ******************************************************************************

#pragma once

#include <iKan/Renderer/Camera/Camera.h>

namespace iKan {
    
    // ******************************************************************************
    // Scene Camera
    // ******************************************************************************
    class SceneCamera : public Camera
    {
    public:
        friend class SceneHeirarchyPannel;

        enum class ProjectionType { Perspective = 0, Orthographic = 1};
        
    public:
        SceneCamera();
        virtual ~SceneCamera() = default;
        
        void SetOrthographic(float size, float nearClip, float farClip);
        void SetPerspective(float fov, float nearClip, float farClip);
        
        void SetViewportSize(uint32_t width, uint32_t height);
        
        float GetPerspectiveFOV() const { return m_PerspectiveFOV; }
        void SetPerspectiveFOV(float fov) { m_PerspectiveFOV = fov; RecalculateProjection(); }
        float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
        void SetPerspectiveNearClip(float near) { m_PerspectiveNear = near; RecalculateProjection(); }
        float GetPerspectiveFarClip() const { return m_PerspectiveFar; }
        void SetPerspectiveFarClip(float far) { m_PerspectiveFar = far; RecalculateProjection(); }
        
        float GetOrthographicSize() const { return m_OrthographicSize; }
        void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }
        float GetOrthographicNearClip() const { return m_OrthographicNear; }
        void SetOrthographicNearClip(float near) { m_OrthographicNear = near; RecalculateProjection(); }
        float GetOrthographicFarClip() const { return m_OrthographicFar; }
        void SetOrthographicFarClip(float far) { m_OrthographicFar = far; RecalculateProjection(); }
        
        ProjectionType GetProjectionType() { return m_ProjectionType; }
        void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjection();}
        
    private:
        void RecalculateProjection();
        
    private:
        float m_OrthographicSize = 10.0f;
        float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;
        
        float m_PerspectiveFOV = glm::radians(45.0f);
        float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;
        
        float m_AspectRatio = 16.0f / 9.0f;
        
        ProjectionType m_ProjectionType = ProjectionType::Perspective;
    };
    
}
