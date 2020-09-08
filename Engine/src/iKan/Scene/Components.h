#pragma once

#include <iKan/Camera/Camera.h>

#include <iKan/Scene/SceneCamera.h>
#include <iKan/Scene/ScriptableEntity.h>

#include <iKan/Renderer/Texture.h>

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
        
        void SetTransform(const glm::vec3& position, const glm::vec3& size = glm::vec3(1.0f))
        {
            Transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), size);
        }

        void SetTransform(const glm::vec2& position, const glm::vec2& size = glm::vec2(1.0f))
        {
            SetTransform({ position.x, position.y, 0.0f }, { size.x, size.y, 1.f});
        }
        
        operator glm::mat4()& { return Transform; }
        operator const glm::mat4&() const { return Transform; }
    };
    
    struct SpriteRendererComponent
    {
        glm::vec4 Color = glm::vec4(1.0f);
        std::shared_ptr<SubTexture> SubTexComp = nullptr;
        
        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        
        SpriteRendererComponent(const glm::vec4& color)
        : Color(color) {}
        
        SpriteRendererComponent(const std::shared_ptr<SubTexture>& subtexture)
        : SubTexComp(subtexture) {}
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
            InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
            DestroyScript     = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
        }
    };
    
}
