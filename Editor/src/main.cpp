#include <iKan.h>

int main(int argc, const char * argv[])
{
    iKan::Log::Init();
    IK_CORE_INFO("Initialized spd logger");
    
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
    
    IK_CORE_INFO("OpenGl Info :");
    IK_CORE_INFO("Vendor   : {0} ", glGetString(GL_VENDOR));
    IK_CORE_INFO("Renderer : {0} ", glGetString(GL_RENDERER));
    IK_CORE_INFO("Version  : {0} ", glGetString(GL_VERSION));
    
    // set the view Port
    glViewport(0, 0, width, height);
    
    // GLFW Callbacks foe events
    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
                              {
        glViewport(0, 0, width, height);
    });
    
    glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
                               {
    });
    
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
                       {
    });
    
    glfwSetCharCallback(window, [](GLFWwindow* window, uint32_t keycode)
                        {
    });
    
    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
                               {
    });
    
    glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset)
                          {
    });
    
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos)
                             {
    });
    
    float vertices[] =
    {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    
    // Creating Vertex Array
    uint32_t VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    // creating memory on the GPU where we store the vertex data
    uint32_t VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // lInking vertex Attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          3 * sizeof(float),
                          (void*)0);
    
    std::string vertexSrc = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    
    void main()
    {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
    )";
    
    std::string fragmentSrc = R"(
    #version 330 core
    out vec4 FragColor;
    
    void main()
    {
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    }
    )";
    
    // Creating Vertex Shader
    uint32_t vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    
    // Attch the shader source and then compile
    const char* vertecString = vertexSrc.c_str();
    glShaderSource(vertexShader, 1, &vertecString, nullptr);
    glCompileShader(vertexShader);
    
    // Error Handling
    GLint isVertexCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isVertexCompiled);
    if (isVertexCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
        
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
        
        glDeleteShader(vertexShader);
        
        IK_CORE_ERROR("{0}", infoLog.data());
        IK_CORE_ASSERT(false, "Shader compilation failure!");
    }
    
    // Creating VertFragmentex Shader
    uint32_t fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Attch the shader source and then compile
    const char* fragmentString = fragmentSrc.c_str();
    glShaderSource(fragmentShader, 1, &fragmentString, nullptr);
    glCompileShader(fragmentShader);
    
    // Error Handling
    GLint isFragmentCompiled = 0;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isFragmentCompiled);
    if (isFragmentCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
        
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
        
        glDeleteShader(fragmentShader);
        
        IK_CORE_ERROR("{0}", infoLog.data());
        IK_CORE_ASSERT(false, "Shader compilation failure!");
    }
    
    // Create Shader program to activate and linke the shader
    uint32_t shaderProgram;
    shaderProgram = glCreateProgram();
    
    // Attach both shader and link them
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // Error Handling
    // Note the different functions here: glGetProgram* instead of glGetShader
    GLint isLinked = 0;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);
        
        /* The maxLength includes the NULL character */
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, &infoLog[0]);
        
        /* We don't need the program anymore. */
        glDeleteProgram(shaderProgram);
        
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        
        IK_CORE_ERROR("{0}", infoLog.data());
        IK_CORE_ASSERT(false, "Shader link failure!");
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    // Bind the shader
    glUseProgram(shaderProgram);
    
    // Game Loop
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Terminate the window after the Ending of game
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
