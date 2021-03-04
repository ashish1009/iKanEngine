// ******************************************************************************
//   File    : Camera.h
//   Project : i-Kan : Renderer
//
//   Created by Ashish
// ******************************************************************************

#pragma once

namespace iKan {
    
    // ******************************************************************************
    // Base class for Camera
    // ******************************************************************************
    class Camera
    {
    public:
        Camera() = default;
        Camera(const glm::mat4& projection)
        : m_Projection(projection) {}
        
        virtual ~Camera() = default;
        
        const glm::mat4& GetProjection() const { return m_Projection; }
        
        // TODO: add API like SetPerspective, Set Ortho ......
        
    protected:
        glm::mat4 m_Projection = glm::mat4(1.0f);
    };
    
}
