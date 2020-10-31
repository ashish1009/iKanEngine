#pragma once

#include <iKan/Renderer/Camera/Camera.h>
#include <iKan/Renderer/Graphics/Mesh.h>

#include <iKan/Scene/SceneLight.h>

namespace iKan {
    
    struct SceneRendererCamera
    {
        iKan::Camera Camera;        
        glm::mat4 ViewMatrix;
    };
    
    struct SceneRendererLight
    {
        bool IsPresent;
        
        SceneLight Light;
        
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
        static void SetCubemapTexture(const std::string& path);

        static void EndScene();
        static void Shutdown();

        static void Draw(const Ref<Mesh>& mesh, const glm::mat4& transform);
        static void DrawCubemap(const glm::mat4& transform);
    };
    
}
