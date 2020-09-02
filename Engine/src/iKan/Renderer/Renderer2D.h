#pragma once

namespace iKan {
    
#define NumQuadVertex 4
#define NumQuadIndices 6
    
    class Renderer2D
    {
    public:
        static void Init();
        static void BeginScene(const std::string& path);
        static void AddShader(const std::string& path);
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        static void EndScene();
        static void Shutdown();
        
        /* Stats */
        struct Statistics
        {
            uint32_t DrawCalls = 0;
            uint32_t QuadCount = 0;
            
            uint32_t GetTotalVertexCount() { return QuadCount * NumQuadVertex; }
            uint32_t GetTotalIndexCount() { return QuadCount * NumQuadIndices; }
        };
        
        static void ResetStats();
        static Statistics GetStats();

    };
    
}
