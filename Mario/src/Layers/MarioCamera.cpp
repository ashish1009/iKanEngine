#include <Layers/MarioCamera.h>
#include <Layers/SceneMap.h>

#include <NativeScripts/CameraController.h>

namespace iKan {
    
    Entity MarioCamera::m_Entity;
    
    void MarioCamera::Init(Ref<Scene>& scene)
    {
        m_Entity = scene->CreateEntity("Camera");
        m_Entity.AddComponent<CameraComponent>();
        m_Entity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
        // Setting Default start point of Game
        m_Entity.GetComponent<TransformComponent>().Transform[3][0] += CAMERA_START;
    }
        
}
