#pragma once

namespace iKan {
    
    struct RendererStatistics
    {
    public:
        static uint32_t DrawCalls;
        static uint32_t QuadCount;
        
        static void Reset();
        static uint32_t GetTotalVertexCount() { return QuadCount * 4; }
        static uint32_t GetTotalIndexCount() { return QuadCount * 6; }
    };

    
}
