// ******************************************************************************
//   File    : Input.h
//   Project : i-Kan : Core
//
//   Created by Ashish
// ******************************************************************************

#pragma once

#include <iKan/Core/Core.h>
#include <iKan/Core/KeyCode.h>
#include <iKan/Core/MouseCode.h>

namespace iKan {
    
    // ******************************************************************************
    // Interface for handling the Input
    // ******************************************************************************
    class Input
    {
    public:
        static bool IsKeyPressed(KeyCode key);
        static bool IsMouseButtonPressed(MouseCode button);
        
        static bool IsKeyReleased(KeyCode key);
        static bool IsMouseButtonReleased(MouseCode button);
        
        static std::pair<float, float> GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();
    };
    
    static void Test(int a);
    
}
