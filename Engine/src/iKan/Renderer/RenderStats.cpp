#include <iKan/Renderer/RenderStats.h>

namespace iKan {

    uint32_t RendererStatistics::DrawCalls = 0;
    uint32_t RendererStatistics::QuadCount = 0;
    
    void RendererStatistics::Reset()
    {
        DrawCalls = 0;
        QuadCount = 0;
    }
}
