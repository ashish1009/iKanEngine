#include <iKan/Renderer/Texture.h>
#include <iKan/Renderer/RenderAPI.h>

#include <iKan/Core/Core.h>

#include <Platform/OpenGL/OpenGlTexture.h>

namespace iKan {
    
    // ------------------------- Vertex Buffer --------------------------------------------
    std::shared_ptr<Texture> Texture::Create(const std::string& path)
    {
        switch (RenderAPI::GetAPI())
        {
            case API::None   : IK_CORE_ASSERT(false, "Render API not Supporting");
            case API::OpenGL : return std::make_shared<OpenGlTexture>(path); break;
            default          : IK_CORE_ASSERT(false, "Invalid Render API ") break;
        }
        return nullptr;
    }
    
    std::shared_ptr<Texture> Texture::Create(uint32_t width, uint32_t height, void* data, uint32_t size)
    {
        switch (RenderAPI::GetAPI())
        {
            case API::None   : IK_CORE_ASSERT(false, "Render API not Supporting");
            case API::OpenGL : return std::make_shared<OpenGlTexture>(width, height, data, size); break;
            default          : IK_CORE_ASSERT(false, "Invalid Render API ") break;
        }
        return nullptr;
    }
    
}
