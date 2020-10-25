#include <glad/glad.h>

#include <iKan/Platform/OpenGL/OpenGlGraphicsContext.h>
#include <iKan/Core/Core.h>

namespace iKan {
    
    OpneGlGraphicsContext::OpneGlGraphicsContext(GLFWwindow* window)
    : m_Window(window)
    {
        IK_PROFILE_FUNCTION();

        Init();
    }
    
    void OpneGlGraphicsContext::Init()
    {
        IK_PROFILE_FUNCTION();

        // make GLFW Window Context
        glfwMakeContextCurrent(m_Window);

        // Initialize OpenGl (Glad)
        /*
         We pass GLAD the function to load the address of the OpenGL function
         pointers which is OS-specific. GLFW gives us glfwGetProcAddress that
         defines the correct function based on which OS we're compiling for
         */
        bool success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        IK_CORE_ASSERT(success, "Can not initialize the Glad");
        
        IK_CORE_INFO("Created Context of OpenGl :");
        IK_CORE_INFO("--------------------------------------------------");
        IK_CORE_INFO("Vendor   : {0} ", glGetString(GL_VENDOR));
        IK_CORE_INFO("Renderer : {0} ", glGetString(GL_RENDERER));
        IK_CORE_INFO("Version  : {0} ", glGetString(GL_VERSION));
        IK_CORE_INFO("--------------------------------------------------");
    }
    
    void OpneGlGraphicsContext::SwapBuffers()
    {
        IK_PROFILE_FUNCTION();

        // Only for OpenGl
        glfwSwapBuffers(m_Window);
    }
    
}
