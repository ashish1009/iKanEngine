#pragma once

#include <iKan/Events/KeyEvents.h>
#include <iKan/Events/MouseEvents.h>
#include <iKan/Events/ApplicationEvents.h>

#include <iKan/Core/Window.h>
#include <iKan/Core/LayerStack.h>

#include <iKan/ImGui/ImguiLayer.h>

namespace iKan {
    
    class Application
    {
    public:
        Application();
        ~Application();
        
        void OnEvent(Event& event);
        void OnUpdate();
        
        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
        
        Window& GetWindow() { return *m_Window; }
        static Application& Get() { return *s_Instance; }
        
    private:
        void OnWindowResize(WindowResizeEvent& event);
        void OnWindowClose(WindowCloseEvent& event);
        void OnMouseScroll(MouseScrollEvent& event);
        
    private:
        /*
         Make sure the Instance of Window should be at first as in default destructor of Application
         Destructor of Window should called at last after Detaching all Layers, as In Window Destructor
         we are TERMINATING the GLFW Window which might have an GLFW Error which detaching (In case we
         Detaching layer will happen after Terminating the Window)
         */
        std::unique_ptr<Window> m_Window;
        LayerStack m_LayerStack;
        ImguiLayer* m_ImguiLayer; // Not using smart pounter because we are deleting all the layers
        
        bool m_IsRunning = true;
        
        uint32_t m_Shader, m_LightShaderProgram;
        uint32_t m_VAO, m_LightVAO;
        uint32_t m_WhiteTextureId, m_CheckBoardTextureID, m_GridTextureID, m_GrassTextureID;

        static Application* s_Instance;
    };
    
}
