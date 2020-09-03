#include <iKan/Core/Application.h>
#include <iKan/Core/Core.h>
#include <iKan/Core/Input.h>

#include <iKan/Renderer/RenderCommand.h>
#include <iKan/Renderer/Renderer2D.h>

namespace iKan {
    
    Application* Application::s_Instance = nullptr;
    
    Application::Application(const std::string& title, uint32_t widht, uint32_t height)
    {
        IK_CORE_ASSERT(!s_Instance, "Application already exists !!!");
        s_Instance = this;
        
        m_Window = std::make_unique<Window>(WindowProp(title, widht, height));
        m_Window->SetEventCallBack(IK_BIND_EVENT_FN(Application::OnEvent));
        
        RenderCommand::Depth(State::Enable);
        RenderCommand::Blend(State::Enable);
        Renderer2D::Init();
        
        m_ImguiLayer = new ImguiLayer();
        PushOverlay(m_ImguiLayer);
    }
    
    Application::~Application()
    {
        Renderer2D::Shutdown();
    }
    
    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }
    
    void Application::PushOverlay(Layer *layer)
    {
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }
    
    void Application::OnUpdate()
    {
        // Game Loop
        while (m_IsRunning)
        {
            float currentFrame  = glfwGetTime();
            float deltaTime     = currentFrame - m_LastFrame;
            m_LastFrame         = currentFrame;
            
            for (Layer* layer : m_LayerStack)
                layer->OnUpdate(deltaTime);
            
            m_ImguiLayer->Begin();
            for (Layer* layer : m_LayerStack)
                layer->OnImguiRender();
            m_ImguiLayer->End();
                
            m_Window->OnUpdate();
        }
    }
    
    bool Application::OnWindowResize(WindowResizeEvent& event)
    {
        RenderCommand::SetViewPort(event.GetWidth(), event.GetHeight());
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

        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); it++)
        {
            if (event.Handled)
                break;
            (*it)->OnEvent(event);
        }
    }

    
}
