#include <iKan.h>

int main(int argc, const char * argv[])
{
    iKan::Log::Init();
    IK_CORE_INFO("Initialized spd logger");
    
    // Initialize the GLFW
    bool success = glfwInit();
    IK_CORE_ASSERT(success, "Can not initialize the GLFW");
    
    // Configure GLFW
    glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // For Mac Only : TODO: Add platform specific macro ifdef here
    glfwInitHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    // Creating Window
    int width = 1280, height = 720;
    std::string  title = "iKan Engine";
    GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window)
    {
        IK_CORE_CRITICAL("Can not create wWindow");
        glfwTerminate();
        return -1;
    }
    IK_CORE_INFO("Creating Window : {0} ({1}x{2})", title, width, height);
    
    // make GLFW Window Context
    glfwMakeContextCurrent(window);
    
    // Initialize OpenGl (Glad)
    /*
     We pass GLAD the function to load the address of the OpenGL function
     pointers which is OS-specific. GLFW gives us glfwGetProcAddress that
     defines the correct function based on which OS we're compiling for
     */
    success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    IK_CORE_ASSERT(success, "Can not initialize the Glad");
    
    // set the view Port
    glViewport(0, 0, width, height);
    
    // Call back for resize the window event
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    });
    
    // Game Loop
    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Terminate the window after the Ending of game
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
