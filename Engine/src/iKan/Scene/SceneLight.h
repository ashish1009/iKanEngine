#pragma once

namespace iKan {
    
    class SceneLight
    {
    public:
        friend class SceneHeirarchyPannel;
        
        enum class LightType { Direction };
        
    public:
        SceneLight() = default;
        ~SceneLight() = default;
        
        // ------------------------------------------------------ Set Values ----------------------------------------
        void SetAmbientFlag(bool flag) { m_IsAmbient = flag; }
        void SetDiffuseFlag(bool flag) { m_IsDiffuse = flag; }
        void SetSpecularFlag(bool flag) { m_IsSpecular = flag; }
        
        void SetAmbient(const glm::vec3& ambient) { m_Ambient = ambient; }
        void SetDiffuse(const glm::vec3& diffuse) { m_Diffuse = diffuse; }
        void SetSpecular(const glm::vec3& specular) { m_Specular = specular; }
        
        void SetType(LightType type) { m_Type = type; }
        
        // ------------------------------------------------------ Get Values ----------------------------------------
        bool GetAmbientFlag() const { return m_IsAmbient; }
        bool GetDiffuseFlag() const { return m_IsDiffuse; }
        bool GetSpecularFlag() const { return m_IsSpecular; }
        
        const glm::vec3 GetAmbient() const { return m_Ambient; }
        const glm::vec3 GetDiffuse() const { return m_Diffuse; }
        const glm::vec3 GetSpecular() const { return m_Specular; }
        
        LightType GetType() const { return m_Type; }
        
    private:
        LightType m_Type = LightType::Direction;
        
        bool m_IsAmbient = false, m_IsDiffuse = false, m_IsSpecular = false;
        
        glm::vec3 m_Ambient  = { 0.2f, 0.2f, 0.2f };
        glm::vec3 m_Diffuse  = { 0.5f, 0.5f, 0.5f };
        glm::vec3 m_Specular = { 1.0f, 1.0f, 1.0f };
    };
    
}
