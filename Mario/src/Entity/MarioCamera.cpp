#include <Entity/MarioCamera.h>
#include <Entity/SceneMap.h>

#include <NativeScripts/CameraController.h>

namespace iKan {
    
    Entity MarioCamera::m_Entity;
    
    void MarioCamera::Init(Ref<Scene>& scene)
    {
        m_Entity = scene->CreateEntity("Camera");
        auto& camera = m_Entity.AddComponent<CameraComponent>().Camera;
        camera.SetProjectionType(SceneCamera::ProjectionType::Orthographic);
        camera.SetOrthographicSize(20.0);
        
        m_Entity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
    }
        
}
