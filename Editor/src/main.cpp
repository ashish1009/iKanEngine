#include <iKan.h>

#include <stb_image.h>

#include <examples/imgui_impl_opengl3.h>
#include <examples/imgui_impl_glfw.h>

static bool s_Perspective = false;
static float s_AspectRatio = 16.0f/9.0f;

static glm::vec3 s_Position = { 0.0f, 0.0f, 0.0f };
static float s_Rotation = 0.0f;

static glm::vec3 s_PerspectiveCameraPosition = { 0.0f, 0.0f, -3.0f};
static glm::vec3 s_OrthoCameraPosition = { 0.0f, 0.0f, 0.0f};

static glm::vec3 s_CameraRotation = { 0.0f, 0.0f, 0.0f };

static float s_FOV = 45.0f;

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
    int windowWidth = 1280, windowHeight = 720;
    std::string  title = "iKan Engine";
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), nullptr, nullptr);
    if (!window)
    {
        IK_CORE_CRITICAL("Can not create wWindow");
        glfwTerminate();
        return -1;
    }
    IK_CORE_INFO("Creating Window : {0} ({1}x{2})", title, windowWidth, windowHeight);
    
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
    glViewport(0, 0, windowWidth, windowHeight);
    
    // GLFW Callbacks foe events
    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
        s_AspectRatio = (float)width / (float)height;
    });
    
    glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
    });
    
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
    });
    
    glfwSetCharCallback(window, [](GLFWwindow* window, uint32_t keycode) {
    });
    
    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
    });
    
    glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset) {
    });
    
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos) {
    });
    
    // ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    
    /* Setup Dear ImGui style */
    ImGui::StyleColorsDark();
    
    /* When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones. */
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
        
    /* Setup Platform/Renderer bindings */
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
    
    float vertices[] =
    {
        /* Position */          /* Color */                 /* TexCoord */
         0.5f,  0.5f, 0.0f,     0.1f, 0.4f, 0.2f, 1.0f,     0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,     0.2f, 0.3f, 0.4f, 1.0f,     0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f,     0.3f, 0.2f, 0.6f, 1.0f,     1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,     0.4f, 0.1f, 0.8f, 1.0f,     1.0f, 0.0f
    };
    
    unsigned int indices[] =
    {
        0, 1, 3,
        1, 2, 3
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
    
    // Creating Element Buffer
    uint32_t EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // lInking vertex Attributes
    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    
    // Color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)12);

    // TexCoord
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)28);

    std::string vertexSrc = R"(
    #version 330 core
    
    layout (location = 0) in vec3 a_Position;
    layout (location = 1) in vec4 a_Color;
    layout (location = 2) in vec2 a_TexCoord;
    
    out vec4 v_Color;
    out vec2 v_TexCoord;
    
    uniform mat4 u_Model;
    uniform mat4 u_ProjectionView;
    
    void main()
    {
    v_Color     = a_Color;
    v_TexCoord  = a_TexCoord;
    
    gl_Position = u_Model * u_ProjectionView * vec4(a_Position.x, a_Position.y, a_Position.z, 1.0);
    }
    )";
    
    std::string fragmentSrc = R"(
    #version 330 core
    
    out vec4 FragColor;
    
    in vec4 v_Color;
    in vec2 v_TexCoord;
    
    uniform sampler2D u_Texture;
    
    void main()
    {
    FragColor = texture(u_Texture, v_TexCoord) * v_Color;
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
    
    // Texture
    stbi_set_flip_vertically_on_load(1);
    
    int height, width, channel;
    
    stbi_uc* data = nullptr;
    data = stbi_load("../../Editor/assets/textures/Checkerboard.png", &width, &height, &channel, 0);
    
    IK_CORE_ASSERT(data, "Failed to load stbi Image");

    GLenum internalFormat = GL_RGB8;
    GLenum dataFormat     = GL_RGB;
    
    if (4 == channel)
    {
        internalFormat = GL_RGBA8;
        dataFormat     = GL_RGBA;
    }
    else if (3 == channel)
    {
        internalFormat = GL_RGB8;
        dataFormat     = GL_RGB;
    }
    IK_CORE_ASSERT((internalFormat & dataFormat), "invalid Format ");
    
    uint32_t textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
    
    if (data)
        stbi_image_free(data);
    
    // Upload Texture to shader at slot 0
    int location = glGetUniformLocation(shaderProgram, "u_Texture");
    
    if (-1 == location)
        IK_CORE_WARN("Warning: uniform '{0}' doesnt exist!", "u_Texture");
    
    glUniform1i(location, 0);
    
    // Game Loop
    while (!glfwWindowShouldClose(window))
    {
        glm::mat4 projection = glm::mat4(1.0f);
        if(s_Perspective)
            projection = glm::perspective(glm::radians(s_FOV), s_AspectRatio, 0.1f, 100.0f);
        else
            projection = glm::ortho(-s_AspectRatio, s_AspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Bind Shader
        glUseProgram(shaderProgram);
        
        // upload model to shader
        glm::mat4 model = glm::translate(glm::mat4(1.0f), s_Position) * glm::rotate(glm::mat4(1.0f), glm::radians(s_Rotation), glm::vec3(1, 0, 0));
        // Upload Texture to shader at slot 0
        location = glGetUniformLocation(shaderProgram, "u_Model");
        
        if (-1 == location)
            IK_CORE_WARN("Warning: uniform '{0}' doesnt exist", "u_Texture");
        
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(model));
        
        glm::mat4 view = glm::mat4(1.0f);
        if (s_Perspective)
            view = glm::translate(glm::mat4(1.0f), s_PerspectiveCameraPosition) *
            glm::rotate(glm::mat4(1.0f), glm::radians(s_CameraRotation.x), glm::vec3(1, 0, 0)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(s_CameraRotation.y), glm::vec3(0, 1, 0)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(s_CameraRotation.z), glm::vec3(0, 0, 1));

        else
            view = glm::inverse(glm::translate(glm::mat4(1.0f), s_OrthoCameraPosition) *
                                glm::rotate(glm::mat4(1.0f), glm::radians(s_CameraRotation.x), glm::vec3(1, 0, 0)) *
                                glm::rotate(glm::mat4(1.0f), glm::radians(s_CameraRotation.y), glm::vec3(0, 1, 0)) *
                                glm::rotate(glm::mat4(1.0f), glm::radians(s_CameraRotation.z), glm::vec3(0, 0, 1)));

        glm::mat4 projectionView = projection * view;
        location = glGetUniformLocation(shaderProgram, "u_ProjectionView");
        
        if (-1 == location)
            IK_CORE_WARN("Warning: uniform '{0}' doesnt exist", "u_Texture");
        
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionView));
        
        // Bind VertexArray
        glBindVertexArray(VAO);
        
        // Bind Texture
        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        
        // Draw Element
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        // Begin Imgui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        // Render Imgui
        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Separator();
        ImGui::DragFloat3("Model", &s_Position.x);
        ImGui::DragFloat("Rotation X", &s_Rotation);
        ImGui::Separator();
            
        float position[3];
        if (s_Perspective)
            memcpy(position, &s_PerspectiveCameraPosition.x, sizeof(float) * 3);
        else
            memcpy(position, &s_OrthoCameraPosition.x, sizeof(float) * 3);

        ImGui::DragFloat3("Camera Position", position);
        ImGui::DragFloat3("Camera Rotation", &s_CameraRotation.x);
        ImGui::Separator();

        if (ImGui::Checkbox("Perspective Camera", &s_Perspective))
        {
        }
        ImGui::DragFloat("FOV", &s_FOV);
        ImGui::Separator();
        
        // End Imgui
        ImGuiIO& io      = ImGui::GetIO();
        io.DisplaySize   = ImVec2((float)width, (float)height);
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    // Terminate the window after the Ending of game
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
