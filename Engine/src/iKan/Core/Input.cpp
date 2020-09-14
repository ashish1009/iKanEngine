#include <iKan/Core/Input.h>
#include <iKan/Core/Application.h>

namespace iKan {
    
    bool Input::IsKeyPressed(KeyCode key)
    {
        auto status = glfwGetKey(Application::Get().GetWindow().GetNativeWindow(), (int32_t)key);
        return status == GLFW_PRESS || status == GLFW_REPEAT;
    }

    bool Input::IsKeyReleased(KeyCode key)
    {
        auto status = glfwGetKey(Application::Get().GetWindow().GetNativeWindow(), (int32_t)key);
        return status == GLFW_RELEASE;
    }
    
    bool Input::IsMouseButtonPressed(MouseCode button)
    {
        auto status = glfwGetMouseButton(Application::Get().GetWindow().GetNativeWindow(), (int32_t)button);
        return status == GLFW_PRESS;
    }

    bool Input::IsMouseButtonReleased(MouseCode button)
    {
        auto status = glfwGetMouseButton(Application::Get().GetWindow().GetNativeWindow(), (int32_t)button);
        return status == GLFW_RELEASE;
    }

    std::pair<float, float> Input::GetMousePosition()
    {
        double xpos, ypos;
        glfwGetCursorPos(Application::Get().GetWindow().GetNativeWindow(), &xpos, &ypos);
        
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
