#pragma once

namespace iKan {
    
    struct RendererStatistics
    {
    public:
        static uint32_t DrawCalls;
        static uint32_t QuadCount;
        static uint32_t VertexCount;
        static uint32_t IndexCount;
        static uint32_t TextureCount;
        
        static void Reset();
    };

    
}
