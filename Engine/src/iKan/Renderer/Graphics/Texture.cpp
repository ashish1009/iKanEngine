#include <iKan/Renderer/Graphics/Texture.h>

#include <iKan/Renderer/API/RendererAPI.h>

#include <iKan/Core/Core.h>

#include <iKan/Platform/OpenGL/OpenGlTexture.h>

namespace iKan {
    
    // ------------------------- Vertex Buffer --------------------------------------------
    Ref<Texture> Texture::Create(const std::string& path)
    {
        switch (RendererAPI::GetAPI())
        {
            case API::None   : IK_CORE_ASSERT(false, "Render API not Supporting");
            case API::OpenGL : return Ref<OpenGlTexture>::Create(path); break;
            default          : IK_CORE_ASSERT(false, "Invalid Render API ") break;
        }
        return nullptr;
    }
    
    Ref<Texture> Texture::Create(uint32_t width, uint32_t height, void* data, uint32_t size)
    {
        switch (RendererAPI::GetAPI())
        {
            case API::None   : IK_CORE_ASSERT(false, "Render API not Supporting");
            case API::OpenGL : return Ref<OpenGlTexture>::Create(width, height, data, size); break;
            default          : IK_CORE_ASSERT(false, "Invalid Render API ") break;
        }
        return nullptr;
    }
    
    SubTexture::SubTexture(const Ref<Texture>& texture, const glm::vec2& min, const glm::vec2& max, const glm::vec2& spriteSize, const glm::vec2& cellSize, const glm::vec2& coords)
    : m_Texture(texture), m_SpriteSize(spriteSize), m_CellSize(cellSize), m_Coords(coords)
    {
        m_TexCoord[0] = {min.x, min.y};
        m_TexCoord[1] = {max.x, min.y};
        m_TexCoord[2] = {max.x, max.y};
        m_TexCoord[3] = {min.x, max.y};
    }
    
    Ref<SubTexture> SubTexture::CreateFromCoords(const Ref<Texture>& texture, const glm::vec2& coords, const glm::vec2& spriteSize, const glm::vec2& cellSize)
    {
        glm::vec2 min = { (coords.x * cellSize.x) / texture->GetWidth(), (coords.y * cellSize.y) / texture->GetHeight() };
        glm::vec2 max = { ((coords.x + spriteSize.x) * cellSize.x) / texture->GetWidth(), ((coords.y + spriteSize.y) * cellSize.y) / texture->GetHeight() };
        return Ref<SubTexture>::Create(texture, min, max, spriteSize, cellSize, coords);
    }
    
    Ref<CubeMapTexture> CubeMapTexture::Create(std::vector<std::string> paths)
    {
        switch (RendererAPI::GetAPI())
        {
            case API::None   : IK_CORE_ASSERT(false, "Render API not Supporting");
            case API::OpenGL : return Ref<OpenGlCubeMapTexture>::Create(paths); break;
            default          : IK_CORE_ASSERT(false, "Invalid Render API ") break;
        }
        return nullptr;
    }
    
    Ref<CubeMapTexture> CubeMapTexture::Create(const std::string& path)
    {
        switch (RendererAPI::GetAPI())
        {
            case API::None   : IK_CORE_ASSERT(false, "Render API not Supporting");
            case API::OpenGL : return Ref<OpenGlCubeMapTexture>::Create(path); break;
            default          : IK_CORE_ASSERT(false, "Invalid Render API ") break;
        }
        return nullptr;
    }
    
}
