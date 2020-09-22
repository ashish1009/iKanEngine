#include <iKan/Renderer/Material.h>

namespace iKan {
    
    Ref<Material> Material::Create(const Ref<Shader>& shader)
    {
        return Ref<Material>::Create(shader);
    }
    
    Material::Material(const Ref<Shader>& shader)
    : m_Shader(shader)
    {
        AllocateStorage();
    }

    void Material::AllocateStorage()
    {
     
    }

    
}
