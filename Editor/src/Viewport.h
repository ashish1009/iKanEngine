// ******************************************************************************
//   File    : Viewport.h
//   Project : Editor
//
//   Created by Ashish
// ******************************************************************************

#pragma once
#include <iKan.h>

namespace iKan {
    
    // ******************************************************************************
    // Store and Update Viewport info
    // ******************************************************************************
    class Viewport
    {
    private:
        Viewport() = default;
    
    public:
        ~Viewport() = default;
        
        Viewport(const Viewport& other) = delete;
        Viewport(Viewport&& other) = delete;
        
        Viewport& operator =(const Viewport& other) = delete;
        Viewport& operator =(Viewport&& other) = delete;
        
        void UpdateBounds();
        void OnUpdate();
        void UpdateMousePos();
        
        static Viewport& Get()
        {
            static Viewport viewport;
            return viewport;
        }
        
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
    };
    
}
