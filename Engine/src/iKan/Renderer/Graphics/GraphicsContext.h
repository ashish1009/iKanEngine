// ******************************************************************************
//   File    : GraphoicsContext.h
//   Project : i-Kan : Renderer
//
//   Created by Ashish
// ******************************************************************************

#pragma once

#include <GLFW/glfw3.h>

namespace iKan {
    
    // ******************************************************************************
    // Interface for Graphics context
    // ******************************************************************************
    class GraphicsContext
    {
    public:
        virtual ~GraphicsContext() = default;
        
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
        
        static Scope<GraphicsContext> CreateContext(GLFWwindow* window);
    };
    
}
