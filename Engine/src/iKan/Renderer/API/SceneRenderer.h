#pragma once

#include <iKan/Editor/EditorCamera.h>
#include <iKan/Renderer/Graphics/Mesh.h>

namespace iKan {
    
    struct SceneRendererCamera
    {
        iKan::Camera Camera;
        
        glm::mat4 ViewMatrix;
    };
    
    class SceneRenderer
    {
    public:
        static void Init();
        static void BegineScene(const SceneRendererCamera& camera);
        static void EndScene();
        static void Shutdown();

        static void Draw(const Ref<Mesh>& mesh, const glm::mat4& transform);
    };
    
}
