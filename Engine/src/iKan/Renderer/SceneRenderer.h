#pragma once

#include <iKan/Renderer/Camera.h>
#include <iKan/Renderer/Mesh.h>

#include <iKan/Scene/Scene.h>

namespace iKan {

    struct SceneRendererCamera
    {
        Camera    Camera;
        
        glm::mat4 ViewMatrix;
        glm::vec3 Front;
    };

    class SceneRenderer
    {
    public:
        static void Init();
        static void Shutdown();
        
        static void BeginScene(const Ref<Scene>& scene, const SceneRendererCamera& camera);
        
        static void DrawMesh(const Ref<Mesh>& mesh, const glm::mat4& transform, bool isADS = true, bool isLightSOurce = false);
        
        static void EndScene();
        static void Flush();
        
        static void SetShaader(const std::string& path);
            
    private:
        static void FlushAndReset();
    };
    
}
