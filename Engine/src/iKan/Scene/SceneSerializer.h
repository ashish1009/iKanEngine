// ******************************************************************************
//   File    : SceneSerializer.h
//   Project : i-Kan : Scene
//
//   Created by Ashish
// ******************************************************************************

#pragma once

#include <iKan/Scene/Scene.h>

namespace iKan {

    // ******************************************************************************
    // cl;ass to save the scene
    // ******************************************************************************
    class SceneSerializer
    {
    public:
        SceneSerializer(const Ref<Scene>& scene);
        
        void Serialize(const std::string& filepath);
        void SerializeRuntime(const std::string& filepath);
        
        bool Deserialize(const std::string& filepath);
        bool DeserializeRuntime(const std::string& filepath);
    private:
        Ref<Scene> m_Scene;
    };
    
}
