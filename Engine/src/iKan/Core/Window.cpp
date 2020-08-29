#include <iKan/Core/Window.h>
#include <iKan/Core/Core.h>

#include <iKan/Events/ApplicationEvents.h>
#include <iKan/Events/KeyEvents.h>
#include <iKan/Events/MouseEvents.h>

namespace iKan {
    
    Window::Window(const WindowProp& prop)
    {
        Init(prop);
    }
    
    void Window::Init(const WindowProp& prop)
    {
        m_Data.Width  = prop.Width;
        m_Data.Height = prop.Height;
        m_Data.Title  = prop.Title;
        
        // Initialize the GLFW
        bool success = glfwInit();
        IK_CORE_ASSERT(success, "Can not initialize the GLFW");
        
        // Configure GLFW
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        // For Mac Only : TODO: Add platform specific macro ifdef here
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        
        // Creating Window
        m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
        if (!m_Window)
        {
            IK_CORE_CRITICAL("Can not create wWindow");
            glfwTerminate();
            return;
        }
        IK_CORE_INFO("Creating Window : {0} ({1}x{2})", m_Data.Title, m_Data.Width, m_Data.Height);
        
        m_Context = GraphicsContext::CreateContext(m_Window);
        glfwSetWindowUserPointer(m_Window, &m_Data);
        
        SetCallBacks();
    }
    
    void Window::SetCallBacks()
    {
        // GLFW Callbacks foe events
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.Width = width;
            data.Height = height;
            
            WindowResizeEvent event(width, height);
            data.EventFunc(event);
        });
        
        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            
            WindowCloseEvent event;
            data.EventFunc(event);
        });
        
        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key, 0);
                    data.EventFunc(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    data.EventFunc(event);
                    break;
                }
                    
                case GLFW_REPEAT:
                {
                    int count = 1;
                    KeyPressedEvent event(key, count);
                    data.EventFunc(event);
                    break;
                }
                    
                default:
                    break;
            }
        });
        
        glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32_t keycode)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            
            KeyTypeEvent event(keycode);
            data.EventFunc(event);
        });
        
        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            
            switch (action)
            {
                case GLFW_PRESS:
                {
                    MouseButtonPressEvent event(button);
                    data.EventFunc(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonPressEvent event(button);
                    data.EventFunc(event);
                    break;
                }
                    
                default:
                    break;
            }
        });
        
        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseScrollEvent event(xOffset, yOffset);
            data.EventFunc(event);
        });
        
        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            
            MouseMoveEvent event(xPos, yPos);
            data.EventFunc(event);

        });
    }
    
    Window::~Window()
    {
        Shutdown();
    }
    
    void Window::OnUpdate()
    {
        m_Context->SwapBuffers();
        glfwPollEvents();
    }
    
    void Window::Shutdown()
    {
        // Terminate the window after the Ending of game
        glfwDestroyWindow(m_Window);
        IK_CORE_INFO("Window : {0} Destroyed", m_Data.Title);
        glfwTerminate();
    }
    
}
