#include "MarioCamera.h"
#include "SceneMap.h"

namespace iKan {
    
    float MarioCamera::s_Speed = 10.0f;
    
    void MarioCamera::Init(const std::shared_ptr<Scene>& scene)
    {
        Entity cameraEntity = scene->CreateEntity("Camera");
        cameraEntity.AddComponent<CameraComponent>();
        
        // Temporary
        class CameraController : public ScriptableEntity
        {
        public:
            void OnUpdate(TimeStep ts)
            {
                auto& transform = GetComponent<TransformComponent>().Transform;
                float speed = s_Speed;
                
                auto& position = transform[3];
                if(Input::IsKeyPressed(Key::Left))
                    position[0] -= speed * ts;
                if(Input::IsKeyPressed(Key::Right))
                    position[0] += speed * ts;
            }
        };
        
        cameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
    }
    
    void MarioCamera::ImGuiRender()
    {
        ImGuiAPI::Counter("Camera Speed", s_Speed);
    }
    
}
