#include <iKan/Renderer/RenderStats.h>

namespace iKan {

    uint32_t RendererStatistics::DrawCalls   = 0;
    uint32_t RendererStatistics::QuadCount   = 0;
    uint32_t RendererStatistics::VertexCount = 0;
    uint32_t RendererStatistics::IndexCount  = 0;
    uint32_t RendererStatistics::TextureCount = 0;
    
    void RendererStatistics::Reset()
    {
        DrawCalls    = 0;
        QuadCount    = 0;
        VertexCount  = 0;
        IndexCount   = 0;
        TextureCount = 0;
    }
}
