// ******************************************************************************
//   File    : SceneSerializer.\cpp
//   Project : i-Kan : Scene
//
//   Created by Ashish
// ******************************************************************************

#include <iKan/Scene/SceneSerializer.h>

#include <iKan/Scene/Entity.h>
#include <iKan/Scene/Components.h>

#pragma clang diagnostic ignored "-Wdocumentation"
#include <yaml-cpp/yaml.h>

namespace YAML {
    
    // ******************************************************************************
    // yml converstions
    // ******************************************************************************
    template<>
    struct convert<glm::vec3>
    {
        static Node encode(const glm::vec3& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }
        
        static bool decode(const Node& node, glm::vec3& rhs)
        {
            if (!node.IsSequence() || node.size() != 3)
                return false;
            
            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };
    
    // ******************************************************************************
    // yml converstions
    // ******************************************************************************
    template<>
    struct convert<glm::vec4>
    {
        static Node encode(const glm::vec4& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }
        
        static bool decode(const Node& node, glm::vec4& rhs)
        {
            if (!node.IsSequence() || node.size() != 4)
                return false;
            
            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };
    
}
namespace iKan {
    
    // ******************************************************************************
    // yml operators
    // ******************************************************************************
    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        return out;
    }
    
    // ******************************************************************************
    // yml operators
    // ******************************************************************************
    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
        return out;
    }
    
    // ******************************************************************************
    // SceneSerializer constructor
    // ******************************************************************************
    SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
    : m_Scene(scene)
    {
        IK_CORE_INFO("Serialising a scene ...");
    }
    
    // ******************************************************************************
    // Serialize the entity. Extract the entity componenets from serialiser
    // ******************************************************************************
    static void SerializeEntity(YAML::Emitter& out, Entity entity)
    {
        UUID uuid = entity.GetComponent<IDComponent>().ID;

        out << YAML::BeginMap; // Entity
        out << YAML::Key << "Entity" << YAML::Value << uuid; // TODO: Entity ID goes here
        
        if (entity.HasComponent<TagComponent>())
        {
            out << YAML::Key << "TagComponent";
            out << YAML::BeginMap; // TagComponent
            
            auto& tag = entity.GetComponent<TagComponent>().Tag;
            out << YAML::Key << "Tag" << YAML::Value << tag;
            
            out << YAML::EndMap; // TagComponent
        }
        
        if (entity.HasComponent<TransformComponent>())
        {
            out << YAML::Key << "TransformComponent";
            out << YAML::BeginMap; // TransformComponent
            
            auto& tc = entity.GetComponent<TransformComponent>();
            out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
            out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
            out << YAML::Key << "Scale" << YAML::Value << tc.Scale;
            
            out << YAML::EndMap; // TransformComponent
        }
        
        if (entity.HasComponent<CameraComponent>())
        {
            out << YAML::Key << "CameraComponent";
            out << YAML::BeginMap; // CameraComponent
            
            auto& cameraComponent = entity.GetComponent<CameraComponent>();
            auto& camera = cameraComponent.Camera;
            
            out << YAML::Key << "Camera" << YAML::Value;
            out << YAML::BeginMap; // Camera
            out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
            out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveFOV();
            out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
            out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
            out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
            out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
            out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
            out << YAML::EndMap; // Camera
            
            out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
            out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;
            
            out << YAML::EndMap; // CameraComponent
        }
        
        if (entity.HasComponent<SpriteRendererComponent>())
        {
            out << YAML::Key << "SpriteRendererComponent";
            out << YAML::BeginMap; // SpriteRendererComponent
            
            auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
            out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;
            
            out << YAML::Key << "TilingFactor" << YAML::Value << spriteRendererComponent.TilingFactor;
            
            if (spriteRendererComponent.Texture)
                out << YAML::Key << "TexAssetPath" << YAML::Value << spriteRendererComponent.Texture->GetfilePath();
            else
                out << YAML::Key << "TexAssetPath" << YAML::Value << "";

            // TODO: Add subtexture later
            out << YAML::EndMap; // SpriteRendererComponent
        }
        
        if (entity.HasComponent<MeshComponent>())
        {
            out << YAML::Key << "MeshComponent";
            out << YAML::BeginMap; // MeshComponent

            auto& mesh = entity.GetComponent<MeshComponent>().Mesh;
            out << YAML::Key << "AssetPath" << YAML::Value << mesh->GetFilepath();

            out << YAML::EndMap; // MeshComponent
        }
        
        if (entity.HasComponent<LightComponent>())
        {
            out << YAML::Key << "LightComponent";
            out << YAML::BeginMap; // LightComponent
            
            auto& lightComponent = entity.GetComponent<LightComponent>();
            auto& light = lightComponent.Light;
            
            out << YAML::Key << "Light" << YAML::Value;
            out << YAML::BeginMap; // Light

            out << YAML::Key << "LightType" << YAML::Value << (int)light.GetType();

            out << YAML::Key << "Is Ambient" << YAML::Value << (bool)light.GetAmbientFlag();
            out << YAML::Key << "Is Diffuse" << YAML::Value << (bool)light.GetDiffuseFlag();
            out << YAML::Key << "Is Specular" << YAML::Value << (bool)light.GetSpecularFlag();

            out << YAML::Key << "Is Blinn" << YAML::Value << (bool)light.GetBlinnFlag();

            out << YAML::Key << "Ambient" << YAML::Value <<  light.GetAmbient();
            out << YAML::Key << "Diffuse" << YAML::Value <<  light.GetDiffuse();
            out << YAML::Key << "Specular" << YAML::Value << light.GetSpecular();
            
            out << YAML::Key << "Constant" << YAML::Value << light.GetConstant();
            out << YAML::Key << "Quadratic" << YAML::Value << light.GetQuadratic();
            out << YAML::Key << "Linear" << YAML::Value << light.GetLinear();

            out << YAML::Key << "Spot Light Dir" << YAML::Value <<  light.GetSpotLightDir();

            out << YAML::Key << "Cutoff" << YAML::Value << light.GetCutoff();
            out << YAML::Key << "Outer Cutoff" << YAML::Value << light.GetOuterCutoff();
            
            out << YAML::EndMap; // Light
            
            out << YAML::Key << "Is Light" << YAML::Value << lightComponent.IsLight;
            
            out << YAML::EndMap; // LightComponent
        }
        
        out << YAML::EndMap; // Entity
    }
    
    // ******************************************************************************
    // Serialize the Scene each enityt
    // ******************************************************************************
    void SceneSerializer::Serialize(const std::string& filepath)
    {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Scene" << YAML::Value << "Untitled";
        out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
        m_Scene->m_Registry.each([&](auto entityID)
                                 {
            Entity entity = { entityID, m_Scene.Raw() };
            if (!entity)
                return;
            
            SerializeEntity(out, entity);
        });
        out << YAML::EndSeq;
        out << YAML::EndMap;
        
        std::ofstream fout(filepath);
        fout << out.c_str();
    }
    
    // ******************************************************************************
    // Serialize the Scene Runtime
    // ******************************************************************************
    void SceneSerializer::SerializeRuntime(const std::string& filepath)
    {
        // Not implemented
        IK_CORE_ASSERT(false, "Not Implemented Yet");
    }
    
    // ******************************************************************************
    // Deserialize the Scene Runtime
    // ******************************************************************************
    bool SceneSerializer::Deserialize(const std::string& filepath)
    {
        YAML::Node data = YAML::LoadFile(filepath);

        if (!data["Scene"])
            return false;
        
        std::string sceneName = data["Scene"].as<std::string>();
        IK_CORE_TRACE("Deserializing scene '{0}'", sceneName);
        
        auto entities = data["Entities"];
        if (entities)
        {
            for (auto entity : entities)
            {
                uint64_t uuid = entity["Entity"].as<uint64_t>(); // TODO
                
                std::string name;
                auto tagComponent = entity["TagComponent"];
                if (tagComponent)
                    name = tagComponent["Tag"].as<std::string>();
                
                IK_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);
                
                Entity deserializedEntity = m_Scene->CreateEntityWithID(uuid, name);
                
                auto transformComponent = entity["TransformComponent"];
                if (transformComponent)
                {
                    // Entities always have transforms
                    auto& tc = deserializedEntity.GetComponent<TransformComponent>();
                    tc.Translation = transformComponent["Translation"].as<glm::vec3>();
                    tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
                    tc.Scale = transformComponent["Scale"].as<glm::vec3>();
                    
                    IK_CORE_INFO("  Entity Transform:");
                    IK_CORE_INFO("      Translation: {0}, {1}, {2}", tc.Translation.x, tc.Translation.y, tc.Translation.z);
                    IK_CORE_INFO("      Rotation: {0}, {1}, {2}", tc.Rotation.x, tc.Rotation.y, tc.Rotation.z);
                    IK_CORE_INFO("      Scale: {0}, {1}, {2}", tc.Scale.x, tc.Scale.y, tc.Scale.z);
                }
                
                auto cameraComponent = entity["CameraComponent"];
                if (cameraComponent)
                {
                    auto& cc = deserializedEntity.AddComponent<CameraComponent>();
                    
                    auto cameraProps = cameraComponent["Camera"]; // TODO: It was reference
                    cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());
                    
                    cc.Camera.SetPerspectiveFOV(cameraProps["PerspectiveFOV"].as<float>());
                    cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
                    cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());
                    
                    cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
                    cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
                    cc.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());
                    
                    cc.Primary = cameraComponent["Primary"].as<bool>();
                    cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
                    
                    IK_CORE_INFO("  Entity Camera:");
                    IK_CORE_INFO("      Primary: {0}", cc.Primary);
                    IK_CORE_INFO("      FixedAspectRatio: {0}", cc.FixedAspectRatio);
                    IK_CORE_INFO("          Projection Type: {0}", cc.Camera.GetProjectionType());
                    
                    IK_CORE_INFO("              PerspectiveFOV: {0}", cc.Camera.GetPerspectiveFOV());
                    IK_CORE_INFO("              PerspectiveFar: {0}", cc.Camera.GetPerspectiveFarClip());
                    IK_CORE_INFO("              PerspectiveNear: {0}", cc.Camera.GetPerspectiveNearClip());

                    IK_CORE_INFO("              OrthographicSize: {0}", cc.Camera.GetOrthographicSize());
                    IK_CORE_INFO("              OrthographicNear: {0}", cc.Camera.GetOrthographicNearClip());
                    IK_CORE_INFO("              OrthographicFar: {0}",  cc.Camera.GetOrthographicFarClip());
                }
                
                auto spriteRendererComponent = entity["SpriteRendererComponent"];
                if (spriteRendererComponent)
                {
                    // TODO: Add for Texutre
                    auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
                    src.Color = spriteRendererComponent["Color"].as<glm::vec4>();

                    src.TilingFactor = spriteRendererComponent["TilingFactor"].as<float>();
                        
                    std::string texPath = spriteRendererComponent["TexAssetPath"].as<std::string>();
                    if (texPath != "")
                        src.Texture = Texture::Create(texPath);
                    else
                        src.Texture = nullptr;
                    
                    IK_CORE_INFO("  Entity Sprite:");
                    
                    IK_CORE_INFO("      Color: {0}, {1}, {2}", src.Color.x, src.Color.y, src.Color.z);
                    IK_CORE_INFO("      Texture Tiling Factor: {0}", src.TilingFactor);
                    IK_CORE_INFO("      Texture Asset Path: {0}", texPath);
                }
                
                auto meshComponent = entity["MeshComponent"];
                if (meshComponent)
                {
                    std::string meshPath = meshComponent["AssetPath"].as<std::string>();
                    if (!deserializedEntity.HasComponent<MeshComponent>())
                        deserializedEntity.AddComponent<MeshComponent>(Ref<Mesh>::Create(meshPath, deserializedEntity));

                    IK_CORE_INFO("  Entity Mesh:");
                    IK_CORE_INFO("      Mesh Asset Path: {0}", meshPath);
                }
                
                auto lightComponent = entity["LightComponent"];
                if (lightComponent)
                {
                    auto& lc = deserializedEntity.AddComponent<LightComponent>();
                    
                    auto lightProps = lightComponent["Light"]; // TODO: It was reference
                    
                    lc.Light.SetType((SceneLight::LightType)lightProps["LightType"].as<int>());

                    lc.Light.SetAmbientFlag(lightProps["Is Ambient"].as<bool>());
                    lc.Light.SetDiffuseFlag(lightProps["Is Diffuse"].as<bool>());
                    lc.Light.SetSpecularFlag(lightProps["Is Specular"].as<bool>());

                    lc.Light.SetBlinnFlag(lightProps["Is Blinn"].as<bool>());

                    lc.Light.SetAmbient(lightProps["Ambient"].as<glm::vec3>());
                    lc.Light.SetDiffuse(lightProps["Diffuse"].as<glm::vec3>());
                    lc.Light.SetSpecular(lightProps["Specular"].as<glm::vec3>());
                    
                    lc.Light.SetConstant(lightProps["Constant"].as<float>());
                    lc.Light.SetQuadratic(lightProps["Quadratic"].as<float>());
                    lc.Light.SetLinear(lightProps["Linear"].as<float>());
 
                    lc.Light.SetSpotLightDir(lightProps["Spot Light Dir"].as<glm::vec3>());

                    lc.Light.SetCutoff(lightProps["Cutoff"].as<float>());
                    lc.Light.SetOuterCutoff(lightProps["Outer Cutoff"].as<float>());
                    
                    lc.IsLight = lightComponent["Is Light"].as<bool>();
                    
                    IK_CORE_INFO("  Entity Light:");
                    IK_CORE_INFO("      Is Light: {0}", lc.IsLight);
                    
                    IK_CORE_INFO("          Is Type: {0}",  lc.Light.GetType());
                    
                    IK_CORE_INFO("          Is Ambient: {0}",  lc.Light.GetAmbientFlag());
                    IK_CORE_INFO("          Is Diffuse: {0}",  lc.Light.GetDiffuseFlag());
                    IK_CORE_INFO("          Is Specular: {0}", lc.Light.GetSpecularFlag());

                    IK_CORE_INFO("          Is Blinn: {0}",  lc.Light.GetBlinnFlag());

                    IK_CORE_INFO("          Ambient: {0}, {1}, {2}",  lc.Light.GetAmbient().x, lc.Light.GetAmbient().y, lc.Light.GetAmbient().z);
                    IK_CORE_INFO("          Diffuse: {0}, {1}, {2}",  lc.Light.GetDiffuse().x, lc.Light.GetDiffuse().y, lc.Light.GetDiffuse().z);
                    IK_CORE_INFO("          Specular: {0}, {1}, {2}", lc.Light.GetSpecular().x, lc.Light.GetSpecular().y, lc.Light.GetSpecular().z);
                    
                    IK_CORE_INFO("          Constant: {0}",  lc.Light.GetConstant());
                    IK_CORE_INFO("          Quadratic: {0}",  lc.Light.GetQuadratic());
                    IK_CORE_INFO("          Linear: {0}", lc.Light.GetLinear());
                    
                    IK_CORE_INFO("          Spot Light Direction: {0}, {1}, {2}",  lc.Light.GetSpotLightDir().x, lc.Light.GetSpotLightDir().y, lc.Light.GetSpotLightDir().z);
                    
                    IK_CORE_INFO("          Cutoff: {0}",  lc.Light.GetCutoff());
                    IK_CORE_INFO("          Outer Cutoff: {0}", lc.Light.GetOuterCutoff());
                }
                
            }
        }
        
        return true;
    }
    
    bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
    {
        // Not implemented
        IK_CORE_ASSERT(false, "Not Implemented Yet");
        return false;
    }
    
}
