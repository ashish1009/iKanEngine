#pragma once

#include <iKan/Renderer/VertexArray.h>

namespace iKan {
    
    enum class API { None = 0, OpenGL = 1 };
    enum class State { Disable = 0, Enable = 1 };
    
    class RenderAPI
    {
    public:
        virtual ~RenderAPI() = default;

        static API GetAPI() { return s_API; }
        static std::unique_ptr<RenderAPI> Create();
        
        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear() = 0;
        virtual void Depth(State state) = 0;
        virtual void Blend(State state) = 0;
        virtual void SetViewPort(uint32_t widht, uint32_t height) = 0;
        virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;
        
    private:
        static API s_API;
    };
    
}
