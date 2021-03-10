#pragma once
#include <iKan.h>

namespace iKan {
    
    struct Viewport
    {
    public:
        bool Focused = false;
        bool Hovered = false;
        
        float Width  = 0.0f;
        float Height = 0.0f;
        
        int32_t MouseX = -1;
        int32_t MouseY = -1;

        glm::vec2 Size = { 0.0f, 0.0f };
        glm::vec2 Bounds[2];
        
        ImVec2 Offset;
        
        Ref<Framebuffer> FrameBuffer;
        
    public:
        void UpdateBounds();
        void OnUpdate();
        void UpdateMousePos();
    };
    
}
