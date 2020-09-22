#pragma once

#include <iKan/Renderer/Shader.h>

namespace iKan {
    
    class Material : public RefCounted
    {
        friend class MaterialInstance;
    public:
        Material(const Ref<Shader>& shader);
        virtual ~Material() = default;

    public:
        static Ref<Material> Create(const Ref<Shader>& shader);
        
    private:
        void AllocateStorage();
        
    private:
        Ref<Shader> m_Shader;

    };
    
    class MaterialInstance
    {
    public:
        MaterialInstance();
        ~MaterialInstance();
    };
    
}
