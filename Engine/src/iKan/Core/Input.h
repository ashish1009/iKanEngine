#pragma once

#include <iKan/Core/KeyCode.h>
#include <iKan/Core/MouseCode.h>

namespace iKan {
    
    class Input
    {
    public:
        static bool IsKeyPressed(KeyCode key);
        static bool IsMouseButtonPressed(MouseCode key);

        static bool IsKeyReleased(KeyCode key);
        static bool IsMouseButtonReleased(MouseCode key);
        
        static std::pair<float, float> GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();

    };
    
}
