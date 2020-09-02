#pragma once

namespace iKan {
    
    class Renderer2D
    {
    public:
        static void Init();
        static void BeginScene(const std::string& path);
        static void AddShader(const std::string& path);
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        static void EndScene();
        static void Shutdown();
    };
    
}
