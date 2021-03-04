// ******************************************************************************
//   File    : MacInput.cpp
//   Project : i-Kan : Platform
//
//   Created by Ashish
// ******************************************************************************

#include <iKan/Core/Application.h>
#include <iKan/Core/Input.h>
#include <GLFW/glfw3.h>

// ******************************************************************************
// Implementation for handling MAC Input
// ******************************************************************************
namespace iKan {

    bool Input::IsKeyPressed(KeyCode key)
    {
        auto window  = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state   = glfwGetKey(window, static_cast<int32_t>(key));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }
    
    bool Input::IsMouseButtonPressed(MouseCode button)
    {
        auto window  = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state   = glfwGetMouseButton(window, static_cast<int32_t>(button));
        return state == GLFW_PRESS;
    }
    
    bool Input::IsKeyReleased(KeyCode key)
    {
        auto window  = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state   = glfwGetKey(window, static_cast<int32_t>(key));
        return state == GLFW_RELEASE;
    }
    
    bool Input::IsMouseButtonReleased(MouseCode button)
    {
        auto window  = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state   = glfwGetMouseButton(window, static_cast<int32_t>(button));
        return state == GLFW_RELEASE;
    }
    
    std::pair<float, float> Input::GetMousePosition()
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        
        return { (float)xpos, (float)ypos };
    }
    
    float Input::GetMouseX()
    {
        auto[x, y] = GetMousePosition();
        return x;
    }
    
    float Input::GetMouseY()
    {
        auto[x, y] = GetMousePosition();
        return y;
    }
    
}
