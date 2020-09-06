#include "MarioCamera.h"
#include "SceneMap.h"

namespace iKan {
    
    static float s_Speed = 5.0f;

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
                
                if(Input::IsKeyPressed(Key::Left))
                    transform[3][0] -= speed * ts;
                if(Input::IsKeyPressed(Key::Right))
                    transform[3][0] += speed * ts;
            }
        };
        
//        cameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
    }
    
    void MarioCamera::ImGuiRender()
    {
        //-------------------------- Camera Speed --------------------------------
        ImGui::Text("Camera Speed");
        ImGui::SameLine();
        
        // Arrow buttons with Repeater
        static int counter  = 16;
        float spacing       = ImGui::GetStyle().ItemInnerSpacing.x;
        
        ImGui::PushButtonRepeat(true);
        if (ImGui::ArrowButton("##left", ImGuiDir_Left))
            counter--;
        
        ImGui::SameLine(0.0f, spacing);
        if (ImGui::ArrowButton("##right", ImGuiDir_Right))
            counter++;
        
        ImGui::PopButtonRepeat();
        ImGui::SameLine();
        ImGui::Text("%d", counter);
        s_Speed = float(counter);

    }
    
}
