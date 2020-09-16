#pragma once

namespace iKan {
    
    class Texture
    {
    public:
        virtual ~Texture() = default;
        
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;

        virtual void Bind(uint32_t slot = 0) const = 0;
        virtual void Unbind() const = 0;
        
        virtual uint32_t GetRendererID() const = 0;
        
        virtual bool operator==(const Texture& other) const = 0;
        
        static std::shared_ptr<Texture> Create(const std::string& path);
        static std::shared_ptr<Texture> Create(uint32_t width, uint32_t height, void* data, uint32_t size);
    };
    
    class SubTexture
    {
    public:
        SubTexture(const std::shared_ptr<Texture>& texture, const glm::vec2& min, const glm::vec2& max, const glm::vec2& spriteSize = {1.0f, 1.0f}, const glm::vec2& cellSize = {16.0f, 16.0f}, const glm::vec2& coords = {0.0f, 0.0f});
        
        const std::shared_ptr<Texture> GetTeture() const { return m_Texture; }
        const glm::vec2* GetTexCoord() const { return m_TexCoord; }
        const glm::vec2& GetSpriteSize() const { return m_SpriteSize; }
        const glm::vec2& GetCellSize() const { return m_CellSize; }
        const glm::vec2& GetCoords() const { return m_Coords; }
        
        /* By Default Tile size is 16 x 16 */
        static std::shared_ptr<SubTexture> CreateFromCoords(const std::shared_ptr<Texture>& texture, const glm::vec2& coords, const glm::vec2& spriteSize = {1.0f, 1.0f}, const glm::vec2& cellSize = {16.0f, 16.0f});
        
    private:
        std::shared_ptr<Texture> m_Texture;
        glm::vec2                m_TexCoord[4];
        glm::vec2                m_SpriteSize;
        glm::vec2                m_CellSize;
        glm::vec2                m_Coords;
    };

    
}
