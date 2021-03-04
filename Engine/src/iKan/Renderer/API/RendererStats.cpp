// ******************************************************************************
//   File    : RendererStats.cpp
//   Project : i-Kan : Renderer
//
//   Created by Ashish
// ******************************************************************************

#include <iKan/Renderer/API/RendererStats.h>

namespace iKan {

    uint32_t RendererStatistics::DrawCalls   = 0;
    uint32_t RendererStatistics::VertexCount = 0;
    uint32_t RendererStatistics::IndexCount  = 0;
    uint32_t RendererStatistics::TextureCount = 0;
    
    // ******************************************************************************
    // Reset the renderer stats
    // ******************************************************************************
    void RendererStatistics::Reset()
    {
        DrawCalls    = 0;
        VertexCount  = 0;
        IndexCount   = 0;
        TextureCount = 0;
    }

}
