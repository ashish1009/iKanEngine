#include <Layers/MarioCamera.h>
#include <Layers/SceneMap.h>

#include <NativeScripts/CameraController.h>

namespace iKan {
    
    float MarioCamera::s_Speed = 10.0f;
    
    void MarioCamera::Init(const std::shared_ptr<Scene>& scene)
    {
        Entity cameraEntity = scene->CreateEntity("Camera");
        cameraEntity.AddComponent<CameraComponent>();
                
        cameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
    }
    
    void MarioCamera::ImGuiRender()
    {
        ImGuiAPI::Counter("Camera Speed", s_Speed);
    }
    
}
