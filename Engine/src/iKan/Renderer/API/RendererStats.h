// ******************************************************************************
//   File    : RendererStats.h
//   Project : i-Kan : Renderer
//
//   Created by Ashish
// ******************************************************************************

#pragma once

namespace iKan {
    
    // ******************************************************************************
    // Stores the renderer stats
    // ******************************************************************************
    struct RendererStatistics
    {
    public:
        static uint32_t DrawCalls;
        static uint32_t VertexCount;
        static uint32_t IndexCount;
        static uint32_t TextureCount;
        
        static void Reset();
    };

    
}
