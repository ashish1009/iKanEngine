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
    
    SubTexture::SubTexture(const std::shared_ptr<Texture>& texture, const glm::vec2& min, const glm::vec2& max, const glm::vec2& spriteSize)
    : m_Texture(texture), m_SpriteSize(spriteSize)
    {
        m_TexCoord[0] = {min.x, min.y};
        m_TexCoord[1] = {max.x, min.y};
        m_TexCoord[2] = {max.x, max.y};
        m_TexCoord[3] = {min.x, max.y};
    }
    
    std::shared_ptr<SubTexture> SubTexture::CreateFromCoords(const std::shared_ptr<Texture>& texture, const glm::vec2& coords, const glm::vec2& spriteSize, const glm::vec2& cellSize)
    {
        glm::vec2 min = { (coords.x * cellSize.x) / texture->GetWidth(), (coords.y * cellSize.y) / texture->GetHeight() };
        glm::vec2 max = { ((coords.x + spriteSize.x) * cellSize.x) / texture->GetWidth(), ((coords.y + spriteSize.y) * cellSize.y) / texture->GetHeight() };
        return std::make_shared<SubTexture>(texture, min, max, spriteSize);
    }
    
}
