#pragma once

#include <iKan/Renderer/Camera/Camera.h>
#include <iKan/Renderer/Graphics/Mesh.h>

#include <iKan/Scene/SceneLight.h>

namespace iKan {
    
    struct SceneRendererCamera
    {
        iKan::Camera Camera;
        
        glm::vec3 Position;
        glm::mat4 ViewMatrix;
        glm::vec3 Front;      
    };
    
    struct SceneRendererLight
    {
        iKan::SceneLight* Light;
        glm::vec3 Position;
        glm::vec3 ViewPos;
        glm::vec3 CameraFront;
    };
    
    class SceneRenderer
    {
    public:
        static void Init();
        static void BegineScene(const SceneRendererCamera& camera);
        static void SetupLight(const SceneRendererLight& light);
        static void EndScene();
        static void Shutdown();

        static void Draw(const Ref<Mesh>& mesh, const glm::mat4& transform);
    };
    
}
