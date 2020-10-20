#pragma once

//TODO: Remove this include in future
#include <iKan/Scene/Components.h>

#include <iKan/Renderer/Camera.h>
#include <iKan/Renderer/Light.h>
#include <iKan/Renderer/Mesh.h>

#include <iKan/Scene/Scene.h>

namespace iKan {

    struct SceneRendererCamera
    {
        iKan::Camera Camera;
        
        glm::mat4 ViewMatrix;
        glm::vec3 Front;
    };
    
    struct SceneRendererLight
    {
        iKan::Light Light;
    };

    class SceneRenderer
    {
    public:
        static void Init();
        static void Shutdown();
        
        static void BeginScene(const Ref<Scene>& scene, const SceneRendererCamera& camera, const SceneRendererLight& sceneRendererLight);
        static void SetCubeMapTexture(const std::vector<std::string>& paths);
        static void SetCubeMapTexture(const std::string& paths);
        
        static void DrawMesh(const Ref<Mesh>& mesh, const glm::mat4& transform, const MeshComponent::Property& meshProp, const MeshComponent::MaterialType& meshMaterial);
        
        static void EndScene();
        static void Flush();
        
        static void SetShaader(const std::string& path);
        
    private:
        static void InitMeshData();
        static void InitCubeMapData();
            
    private:
        static void FlushAndReset();
    };
    
}
