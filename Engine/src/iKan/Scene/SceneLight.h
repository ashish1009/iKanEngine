// ******************************************************************************
//   File    : SceneLight.h
//   Project : i-Kan : Scene
//
//   Created by Ashish
// ******************************************************************************

#pragma once

namespace iKan {
    
    // ******************************************************************************
    // Scene Light Class
    // ******************************************************************************
    class SceneLight
    {
    public:
        friend class SceneHeirarchyPannel;
        
        enum class LightType { Direction, Point, Spot };
        
    public:
        SceneLight() = default;
        ~SceneLight() = default;
        
        // ******************************************************************************
        // Chanegs default value of type of light is changes
        // ******************************************************************************
        void SetType(LightType type)
        {
            switch (type)
            {
                case LightType::Direction:
                {
                    m_Ambient  = { 0.05f, 0.05f, 0.05f };
                    m_Diffuse  = { 0.4f, 0.4f, 0.4f };
                    m_Specular = { 0.5f, 0.5f, 0.5f };
                    break;
                }
                    
                case LightType::Point:
                {
                    m_Ambient  = { 0.05f, 0.05f, 0.05f };
                    m_Diffuse  = { 0.8f, 0.8f, 0.8f };
                    m_Specular = { 1.0f, 1.0f, 1.0f };
                    
                    m_Constant  = 1.0f;
                    m_Linear    = 0.09f;
                    m_Quadratic = 0.032f;
                    break;
                }
                    
                case LightType::Spot:
                {
                    m_Ambient  = { 0.0f, 0.0f, 0.0f };
                    m_Diffuse  = { 1.0f, 1.0f, 1.0f };
                    m_Specular = { 1.0f, 1.0f, 1.0f };
                    
                    m_Constant  = 1.0f;
                    m_Linear    = 0.09f;
                    m_Quadratic = 0.032f;
                    
                    m_CutOff      = 12.5f;  // glm::cos(glm::radians)
                    m_OuterCutOff = 15.0f;  // glm::cos(glm::radians)
                    break;
                }
                    
                default:
                    break;
            }
            m_Type = type;
        }
        
        void SetAmbientFlag(bool flag) { m_IsAmbient = flag; }
        void SetDiffuseFlag(bool flag) { m_IsDiffuse = flag; }
        void SetSpecularFlag(bool flag) { m_IsSpecular = flag; }

        void SetBlinnFlag(bool flag) { m_IsBlinn = flag; }

        void SetAmbient(const glm::vec3& ambient) { m_Ambient = ambient; }
        void SetDiffuse(const glm::vec3& diffuse) { m_Diffuse = diffuse; }
        void SetSpecular(const glm::vec3& specular) { m_Specular = specular; }
        
        void SetConstant(float value) { m_Constant = value; }
        void SetLinear(float value) { m_Linear = value; }
        void SetQuadratic(float value) { m_Quadratic = value; }
        
        void SetSpotLightDir(const glm::vec3& value) { m_SpotLightDir = value; }

        void SetCutoff(float value) { m_CutOff = value; }
        void SetOuterCutoff(float value) { m_OuterCutOff = value; }
        
        // ------------------------------------------------------ Get Values ----------------------------------------
        bool GetAmbientFlag() const { return m_IsAmbient; }
        bool GetDiffuseFlag() const { return m_IsDiffuse; }
        bool GetSpecularFlag() const { return m_IsSpecular; }
        
        bool GetBlinnFlag() const { return m_IsBlinn; }

        const glm::vec3& GetAmbient() const { return m_Ambient; }
        const glm::vec3& GetDiffuse() const { return m_Diffuse; }
        const glm::vec3& GetSpecular() const { return m_Specular; }
        
        LightType GetType() const { return m_Type; }
        
        float GetConstant() const { return m_Constant; }
        float GetLinear() const { return m_Linear; }
        float GetQuadratic() const { return m_Quadratic; }
        
        const glm::vec3& GetSpotLightDir() const { return m_SpotLightDir; }

        float GetCutoff() const { return m_CutOff; }
        float GetOuterCutoff() const { return m_OuterCutOff; }
        
    private:
        LightType m_Type = LightType::Direction;
        
        bool m_IsAmbient = false, m_IsDiffuse = false, m_IsSpecular = false;
        bool m_IsBlinn = false;
        
        glm::vec3 m_Ambient  = { 0.2f, 0.2f, 0.2f };
        glm::vec3 m_Diffuse  = { 0.5f, 0.5f, 0.5f };
        glm::vec3 m_Specular = { 1.0f, 1.0f, 1.0f };
        
        float m_Constant  = 1.0f;
        float m_Linear    = 0.09f;
        float m_Quadratic = 0.032f;
        
        glm::vec3 m_SpotLightDir = { 0.0f, 0.0f, 0.0f };
        
        float m_CutOff      = 12.5f;  // glm::cos(glm::radians)
        float m_OuterCutOff = 15.0f;  // glm::cos(glm::radians)
    };
    
}
