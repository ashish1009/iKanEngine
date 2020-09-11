#pragma once

#include <iKan.h>

namespace iKan {
    
    class CameraController;
    
    class MarioCamera
    {
    public:
        static void Init(const std::shared_ptr<Scene>& scene);
        
        static std::pair<float&, float&> GetPosition()
        {
            auto& position = m_Entity.GetComponent<TransformComponent>().Transform[3];
            return { position[0], position[1] };
        }
        
    private:
        static Entity m_Entity;
        
        friend class CameraController;
    };
    
}
