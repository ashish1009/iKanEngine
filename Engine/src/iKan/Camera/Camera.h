#pragma once

namespace iKan {
    
    class Camera
    {
    public:
        Camera() = default;
        Camera(const glm::mat4& projection)
        : m_Projection(projection) {}
        
        const glm::mat4& GetProjection() const { return m_Projection; }
        
        // TODO: add API like SetPerspective, Set Ortho ......
        
    private:
        glm::mat4 m_Projection;
    };
    
}
