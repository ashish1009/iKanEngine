#pragma once

#include <iKan/Scene/SceneCamera.h>
#include <iKan/Scene/ScriptableEntity.h>

#include <iKan/Renderer/Texture.h>
#include <iKan/Renderer/Light.h>
#include <iKan/Renderer/Mesh.h>

namespace iKan {
    
    static constexpr float MaterialRefractiveIndex[] = { 1.00f, 1.33f, 1.309, 1.52, 2.42 };
    
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
        Ref<SubTexture> SubTexComp = nullptr;
        
        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        
        SpriteRendererComponent(const glm::vec4& color)
        : Color(color) {}
        
        SpriteRendererComponent(const Ref<SubTexture>& subtexture)
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
        enum class Property
        {
            Basic,
            ADS,
            LightSource,
            Reflection,
            Refraction,
        };
        
        enum class MaterialType
        {
            Air,
            Water,
            Ice,
            Glass,
            Diamond,
        };
        
        Ref<iKan::Mesh> Mesh;
        Property Prop = Property::ADS;
        MaterialType Material = MaterialType::Air;
        
        MeshComponent() = default;
        MeshComponent(const MeshComponent& other) = default;
        MeshComponent(const Ref<iKan::Mesh>& mesh)
        : Mesh(mesh) {}
        
        operator Ref<iKan::Mesh> () { return Mesh; }
    };
    
    struct LightComponent
    {
        iKan::Light Light;
        
        bool Primary = true;
        
        LightComponent() = default;
        LightComponent(const LightComponent&) = default;
    };

}
