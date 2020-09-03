#pragma once

#include <iKan/Camera/Camera.h>

#include <iKan/Scene/SceneCamera.h>
#include <iKan/Scene/ScriptableEntity.h>

namespace iKan {
    
    struct TagComponent
    {
        std::string Tag;
        
        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& tag)
        : Tag(tag) {}
    };
    
    struct TransformComponent
    {
        glm::mat4 Transform{ 1.0 };
        
        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::mat4& transform)
        : Transform(transform) {}
        
        operator glm::mat4()& { return Transform; }
        operator const glm::mat4&() const { return Transform; }
    };
    
    struct SpriteRendererComponent
    {
        glm::vec4 Color = glm::vec4(1.0f);
        
        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& color)
        : Color(color) {}
    };
    
    struct CameraComponent
    {
        SceneCamera Camera;

        bool Primary = true;
        bool FixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
    };
    
    struct NativeScriptComponent
    {
        ScriptableEntity* Instance = nullptr;
        
        ScriptableEntity*(*InstantiateScript)();
        void(*DestroyScript)(NativeScriptComponent*);
        
        template<typename T>
        void Bind()
        {
            DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
            InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
        }
    };
    
}
