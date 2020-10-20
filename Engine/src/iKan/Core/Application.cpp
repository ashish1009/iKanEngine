#include <iKan/Core/Application.h>
#include <iKan/Core/Core.h>
#include <iKan/Core/Input.h>

#include <iKan/Renderer/API/Renderer.h>
#include <iKan/ImGui/ImGuiAPI.h>

#include <GLFW/glfw3.h>

namespace iKan {
    
    Application* Application::s_Instance = nullptr;
    
    Application::Application(const ApplicationProps& props)
    {
        IK_CORE_ASSERT(!s_Instance, "Application already exists !!!");
        s_Instance = this;
        
        // Creating Window from Applicaition
        m_Window = Window::Create(WindowProp(props.Title, props.Width, props.Height));
        m_Window->SetEventCallback(IK_BIND_EVENT_FN(Application::OnEvent));
        
        // Initialising the Renderer
        Renderer::Init();

        m_ImguiLayer = new ImguiLayer();
        PushOverlay(m_ImguiLayer);
    }
    
    Application::~Application()
    {
        Renderer::Shutdown();
    }
    
    void Application::PushLayer(Layer* layer)
    {
        IK_CORE_INFO("Pushing Layer: {0} at {1} ", layer->GetName(), m_LayerStack.GetNumInserted());
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }	
    
    void Application::PushOverlay(Layer *layer)
    {
        IK_CORE_INFO("Pushing Layer: {0} at End ", layer->GetName());
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }
    
    void Application::ImGuiRenderer()
    {
        m_ImguiLayer->Begin();

        // Rendering ImGui for all the layers
        for (Layer* layer : m_LayerStack)
            layer->OnImguiRender();
        
        m_ImguiLayer->End();
    }
    
    void Application::Run()
    {
        // Game Loop
        while (m_IsRunning)
        {
            // Updating all the attached layer
            for (Layer* layer : m_LayerStack)
                layer->OnUpdate(m_TimeStep);
            
            ImGuiRenderer();
            
            // Window Update to refresh
            m_Window->Update();
            
            float currentFrame = glfwGetTime();
            m_TimeStep         = currentFrame - m_LastFrame;
            m_LastFrame        = currentFrame;
        }
    }
    
    bool Application::OnWindowResize(WindowResizeEvent& event)
    {
        Renderer::SetViewport(event.GetWidth(), event.GetHeight());
        return false;
    }

    bool Application::OnWindowClose(WindowCloseEvent& event)
    {
        m_IsRunning = false;
        return false;
    }

    void Application::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        
        dispatcher.Dispatch<WindowResizeEvent>(IK_BIND_EVENT_FN(Application::OnWindowResize));
        dispatcher.Dispatch<WindowCloseEvent>(IK_BIND_EVENT_FN(Application::OnWindowClose));

        // Event callpacks for all layers
        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); it++)
        {
            if (event.Handled)
                break;
            (*it)->OnEvent(event);
        }
    }

    
}
