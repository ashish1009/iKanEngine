#pragma once

#include <iKan/Core/Events/KeyEvents.h>
#include <iKan/Core/Events/MouseEvents.h>
#include <iKan/Core/Events/ApplicationEvents.h>

#include <iKan/Core/Window.h>
#include <iKan/Core/LayerStack.h>

#include <iKan/ImGui/ImguiLayer.h>

namespace iKan {
    
    struct ApplicationProps
    {
        std::string Title = "iKan Engine";
        uint32_t Width    = s_WindowWidth;
        uint32_t Height   = s_WindowHeight;
        
        ApplicationProps() = default;
    };
    
    class Application
    {
    public:
        Application(const ApplicationProps& props = ApplicationProps());
        virtual ~Application();
        
        void OnEvent(Event& event);
        void Run();
        
        void ImGuiRenderer();
        
        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        void SetWindowVSynch(bool flag) { m_Window->SetVSync(flag); } 
        
        void Close() { m_IsRunning = false; }
        
        ImguiLayer* GetImGuiLayer() { return m_ImguiLayer; }
        Window& GetWindow() { return *m_Window; }
        static Application& Get() { return *s_Instance; }
        
    private:
        bool OnWindowResize(WindowResizeEvent& event);
        bool OnWindowClose(WindowCloseEvent& event);
        
    private:
        /*
         Make sure the Instance of Window should be at first as in default destructor of Application
         Destructor of Window should called at last after Detaching all Layers, as In Window Destructor
         we are TERMINATING the GLFW Window which might have an GLFW Error which detaching (In case we
         Detaching layer will happen after Terminating the Window)
         */
        Scope<Window> m_Window;
        
        LayerStack  m_LayerStack;
        ImguiLayer* m_ImguiLayer; // Not using smart pounter because we are deleting all the layers
        TimeStep    m_TimeStep;
        
        bool  m_IsRunning = true;
        float m_LastFrame = 0.0f;
        
        static Application* s_Instance;
    };
    
    Application* CreateApplication();

}
