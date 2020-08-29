#pragma once

#include <iKan/Events/KeyEvents.h>
#include <iKan/Events/MouseEvents.h>
#include <iKan/Events/ApplicationEvents.h>

#include <iKan/Core/Window.h>

namespace iKan {
    
    class Application
    {
    public:
        Application();
        ~Application();
        
        void OnEvent(Event& event);
        void OnUpdate();
        
    private:
        void OnWindowResize(WindowResizeEvent& event);
        void OnWindowClose(WindowCloseEvent& event);
        void OnMouseScroll(MouseScrollEvent& event);
        
    private:
        Window m_Window;
        
        uint32_t m_Shader, m_LightShaderProgram;
        uint32_t m_VAO, m_LightVAO;
        uint32_t m_WhiteTextureId, m_CheckBoardTextureID, m_GridTextureID, m_GrassTextureID;

    };
    
}
