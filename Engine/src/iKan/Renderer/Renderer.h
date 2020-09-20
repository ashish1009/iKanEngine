#pragma once

#include <iKan/Renderer/RenderCommand.h>

namespace iKan {
    
    class Renderer
    {
    public:
        static void Init();
        static void Clear(glm::vec4 color);
        static void SetViewport(float width, float height);
        static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0);
        static void Shutdown();
    };
    
}
