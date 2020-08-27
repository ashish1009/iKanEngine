#include <iKan/Core/Window.h>
#include <iKan/Core/Core.h>

namespace iKan {
    
    Window::Window(const std::string& title, uint32_t width, uint32_t height)
    : m_Width(width), m_Height(height), m_Title(title)
    {
        Init();
    }
    
    void Window::Init()
    {
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
        m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
        if (!m_Window)
        {
            IK_CORE_CRITICAL("Can not create wWindow");
            glfwTerminate();
            return;
        }
        IK_CORE_INFO("Creating Window : {0} ({1}x{2})", m_Title, m_Width, m_Height);
        
        m_Context = GraphicsContext::CreateContext(m_Window);
        
        SetCallBacks();
    }
    
    void Window::SetCallBacks()
    {
        // GLFW Callbacks foe events
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
                                  {
            //            glViewport(0, 0, width, height);
            //            s_AspectRatio = (float)width / (float)height;
        });
        
        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
        });
        
        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        });
        
        glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32_t keycode) {
        });
        
        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
        });
        
        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
                              {
            //            s_CameraRotation.x += yOffset;
            //            s_CameraRotation.y += xOffset;
        });
        
        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
        });
    }
    
    Window::~Window()
    {
        Shutdown();
    }
    
    void Window::Update()
    {
        m_Context->SwapBuffers();
        glfwPollEvents();
    }
    
    void Window::Shutdown()
    {
        // Terminate the window after the Ending of game
        glfwDestroyWindow(m_Window);
        IK_CORE_INFO("Window : {0} Destroyed", m_Title);
        glfwTerminate();
    }
    
}
