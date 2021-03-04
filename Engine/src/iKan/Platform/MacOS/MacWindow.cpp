// ******************************************************************************
//   File    : MacWindow.cpp
//   Project : i-Kan : Platform
//
//   Created by Ashish
// ******************************************************************************

#include <iKan/Platform/MacOS/MacWindow.h>

#include <iKan/Core/Events/MouseEvents.h>
#include <iKan/Core/Events/KeyEvents.h>
#include <iKan/Core/Events/ApplicationEvents.h>

#include <iKan/Core/Input.h>

namespace iKan {
    
    // ******************************************************************************
    // Constructor of MAC Window
    // ******************************************************************************
    MacWindow::MacWindow(const WindowProp& prop)
    {
        IK_CORE_INFO("Constructing MAC Window");

        Init(prop);
    }
    
    // ******************************************************************************
    // Initializing of MAC Window
    // ******************************************************************************
    void MacWindow::Init(const WindowProp &prop)
    {
        m_Data.Title  = prop.Title;
        m_Data.Width  = prop.Width;
        m_Data.Height = prop.Height;
        
        // Initialize the library
        bool initSuccess = glfwInit();
        IK_CORE_ASSERT(initSuccess, "GLFW Init fails!!!!");
        
        // Configure GLFW Context Version
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
#ifdef IK_PLATFORM_MAC
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        // Create a windowed mode window and its OpenGL context
        m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), NULL, NULL);
        if (!m_Window)
        {
            IK_CORE_ASSERT(false, "Unable to create Window!!!!");
            glfwTerminate();
            return;
        }
        IK_CORE_INFO("Creating Window : {0} ({1}x{2})", m_Data.Title, m_Data.Width, m_Data.Height);
        m_Context = GraphicsContext::CreateContext(m_Window);
        
        // Set the pointer of GLFW Window
        glfwSetWindowUserPointer(m_Window, &m_Data);
        
        SetCallBacks();
    }
    
    // ******************************************************************************
    // Set callbacks for AC Window
    // ******************************************************************************
    void MacWindow::SetCallBacks()
    {
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
                                  {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            
            data.Width  = width;
            data.Height = height;
            
            WindowResizeEvent event(width, height);
            data.EventCallback(event);
        });
        
        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
                                   {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.EventCallback(event);
        });
        
        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
                           {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            
            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(static_cast<KeyCode>(key), 0);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(static_cast<KeyCode>(key));
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(static_cast<KeyCode>(key), 1);
                    data.EventCallback(event);
                    break;
                }
            }
        });
        
        glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32_t keycode)
                            {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            
            KeyTypedEvent event(static_cast<KeyCode>(keycode));
            data.EventCallback(event);
        });
        
        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
                                   {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            
            switch (action)
            {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(static_cast<MouseCode>(button));
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(static_cast<MouseCode>(button));
                    data.EventCallback(event);
                    break;
                }
            }
        });
        
        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
                              {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            
            MouseScrolledEvent event((float)xOffset, (float)yOffset);
            data.EventCallback(event);
        });
        
        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
                                 {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            
            MouseMovedEvent event((float)xPos, (float)yPos);
            data.EventCallback(event);
        });
    }
    
    // ******************************************************************************
    // Destrucot of MAC Window
    // ******************************************************************************
    MacWindow::~MacWindow()
    {
        IK_CORE_WARN("Destroying MAC Window");

        Shutdown();
    }
    
    // ******************************************************************************
    // Shutting down MAC Window
    // ******************************************************************************
    void MacWindow::Shutdown()
    {
        IK_CORE_WARN("Shutting down MAC Window");

        glfwDestroyWindow(m_Window);
        IK_CORE_INFO("Window : {0} Destroyed", m_Data.Title);
        glfwTerminate();
    }
    
    // ******************************************************************************
    // Constructor of MAC Window
    // ******************************************************************************
    void MacWindow::Update()
    {
        m_Context->SwapBuffers();
        glfwPollEvents();
    }
    
    // ******************************************************************************
    // Set VSynch
    // ******************************************************************************
    void MacWindow::SetVSync(bool enabled)
    {
        IK_CORE_INFO("Setting VSynch : {0}", enabled);
        (true == enabled) ? glfwSwapInterval(1) : glfwSwapInterval(0);
        m_Data.VSync = enabled;
    }
    
    // ******************************************************************************
    // Get VSynch
    // ******************************************************************************
    bool MacWindow::IsVSync() const
    {
        IK_CORE_INFO("VSynch : {0}", m_Data.VSync);
        return m_Data.VSync;
    }
    
}
