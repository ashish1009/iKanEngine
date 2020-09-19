#include <iKan/Core/Window.h>
#include <iKan/Core/Core.h>
#include <iKan/Core/KeyCode.h>
#include <iKan/Core/MouseCode.h>

#include <iKan/Core/Events/ApplicationEvents.h>
#include <iKan/Core/Events/KeyEvents.h>
#include <iKan/Core/Events/MouseEvents.h>

namespace iKan {
    
    static void GLFWErrorCallback(int error, const char* description)
    {
        IK_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }
    
    static bool s_GLFWInitialized = false;
    
    Window::Window(const WindowProp& prop)
    {
        Init(prop);
    }
    
    void Window::Init(const WindowProp& prop)
    {
        m_Data.Width  = prop.Width;
        m_Data.Height = prop.Height;
        m_Data.Title  = prop.Title;
        
        if (!s_GLFWInitialized)
        {
            // TODO: glfwTerminate on system shutdown
            int success = glfwInit();
            IK_CORE_ASSERT(success, "Could not intialize GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
            
            s_GLFWInitialized = true;
        }

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
                    KeyPressedEvent event((KeyCode)key, 0);
                    data.EventFunc(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event((KeyCode)key);
                    data.EventFunc(event);
                    break;
                }
                    
                case GLFW_REPEAT:
                {
                    int count = 1;
                    KeyPressedEvent event((KeyCode)key, count);
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
            
            KeyTypeEvent event((KeyCode)keycode);
            data.EventFunc(event);
        });
        
        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            
            switch (action)
            {
                case GLFW_PRESS:
                {
                    MouseButtonPressEvent event((MouseCode)button);
                    data.EventFunc(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonPressEvent event((MouseCode)button);
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
        
        m_ImGuiMouseCursors[ImGuiMouseCursor_Arrow]      = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
        m_ImGuiMouseCursors[ImGuiMouseCursor_TextInput]  = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
        m_ImGuiMouseCursors[ImGuiMouseCursor_ResizeAll]  = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);   // FIXME: GLFW doesn't have this.
        m_ImGuiMouseCursors[ImGuiMouseCursor_ResizeNS]   = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
        m_ImGuiMouseCursors[ImGuiMouseCursor_ResizeEW]   = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
        m_ImGuiMouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);  // FIXME: GLFW doesn't have this.
        m_ImGuiMouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);  // FIXME: GLFW doesn't have this.
        m_ImGuiMouseCursors[ImGuiMouseCursor_Hand]       = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
        
        // Update window size to actual size
        {
            int width, height;
            glfwGetWindowSize(m_Window, &width, &height);
            m_Data.Width = width;
            m_Data.Height= height;
        }
    }
    
    Window::~Window()
    {
        Shutdown();
    }
    
    void Window::OnUpdate()
    {
        m_Context->SwapBuffers();
        glfwPollEvents();
        
        ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
        glfwSetCursor(m_Window, m_ImGuiMouseCursors[imgui_cursor] ? m_ImGuiMouseCursors[imgui_cursor] : m_ImGuiMouseCursors[ImGuiMouseCursor_Arrow]);
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        
        float time      = glfwGetTime();
//        float delta     = time - m_LastFrameTime;
        m_LastFrameTime = time;
    }
    
    void Window::Shutdown()
    {
        // Terminate the window after the Ending of game
        glfwDestroyWindow(m_Window);
        IK_CORE_INFO("Window : {0} Destroyed", m_Data.Title);
        glfwTerminate();
    }
    
    inline std::pair<float, float> Window::GetWindowPos() const
    {
        int x, y;
        glfwGetWindowPos(m_Window, &x, &y);
        return { x, y };
    }
    
    void Window::SetVSync(bool enabled)
    {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);
        
        m_Data.VSync = enabled;
    }
    
    bool Window::IsVSync() const
    {
        return m_Data.VSync;
    }

    
}
