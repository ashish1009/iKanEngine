#pragma once

#include <iKan/Core/UUID.h>

#include <iKan/Scene/SceneCamera.h>
#include <iKan/Scene/SceneLight.h>
#include <iKan/Scene/ScriptableEntity.h>

#include <iKan/Renderer/Graphics/Texture.h>
#include <iKan/Renderer/Graphics/Mesh.h>

namespace iKan {
    
    static constexpr float MaterialRefractiveIndex[] = { 1.00f, 1.33f, 1.309, 1.52, 2.42 };
    
    struct IDComponent
    {
        UUID ID = 0;
    };

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
        glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };
        
        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& translation)
        : Translation(translation) {}
        
        inline glm::mat4 GetTransform() const
        {
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1, 0, 0 })
            * glm::rotate(glm::mat4(1.0f), Rotation.y, { 0, 1, 0 })
            * glm::rotate(glm::mat4(1.0f), Rotation.z, { 0, 0, 1 });
            
            return glm::translate(glm::mat4(1.0f), Translation)
            * rotation
            * glm::scale(glm::mat4(1.0f), Scale);
        }
        
    };
    
    struct SpriteRendererComponent
    {
        glm::vec4 Color                  = glm::vec4(1.0f);
        Ref<iKan::SubTexture> SubTexComp = nullptr;
        Ref<iKan::Texture> Texture       = nullptr;
        
        float TilingFactor = 1.0f;
        
        glm::vec2 SpriteSize = { 1.0f, 1.0f };
        glm::vec2 CellSize   = { 16.0f, 16.0f };
        glm::vec2 Coords     = { 0.0f, 0.0f };

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        
        SpriteRendererComponent(const glm::vec4& color)
        : Color(color) {}
        
        SpriteRendererComponent(const Ref<SubTexture>& subtexture)
        : SubTexComp(subtexture), Texture(nullptr) {}
        
        SpriteRendererComponent(const Ref<iKan::Texture>& texture)
        : SubTexComp(nullptr), Texture(texture) {}
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
    public:
        std::vector<ScriptableEntity*> Scripts;
        
        template<typename T>
        void Bind()
        {
            Instance = static_cast<ScriptableEntity*>(new T());
            Scripts.emplace_back(Instance);
        }
        
    private:
        ScriptableEntity* Instance = nullptr;
    };
    
    struct MeshComponent
    {
        Ref<iKan::Mesh> Mesh;
        
        MeshComponent() = default;
        MeshComponent(const MeshComponent&) = default;
        
        MeshComponent(const Ref<iKan::Mesh>& mesh)
        : Mesh(mesh) {}
    };
    
    struct LightComponent
    {
        SceneLight Light;
        
        bool IsLight = false;
        
        LightComponent() = default;
        LightComponent(const LightComponent&) = default;
    };
    
}
