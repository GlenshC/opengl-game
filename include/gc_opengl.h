
#ifndef GC_OPENGL_H
#define GC_OPENGL_H

#define GS_MAIN_INIT \
    if (!glfwInit()) {                                                      \
        GC_LOG("Failed to initialize GLFW.");                               \
        return -1;                                                          \
    }                                                                       \
                                                                            \
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                          \
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                          \
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);          \
                                                                            \
    window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "Hello World", NULL, NULL);\
    if (!window)                                                            \
    {                                                                       \
        GC_LOG("Failed to create window.");                                 \
        glfwTerminate();                                                    \
        return -1;                                                          \
    }                                                                       \
                                                                            \
    glfwMakeContextCurrent(window);                                         \
                                                                            \
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {              \
        GC_LOG("Failed to initialized GLAD.");                              \
        glfwTerminate();                                                    \
        return -1;                                                          \
    }  

#ifdef GC_OPENGL_ENABLE_CALLBACKS

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void Window_FramebufferSizeCallback(GLFWwindow *window, int width, int height);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
static void scroll_callback(GLFWwindow* window, double xpos, double ypos);
static void key_movement(GLFWwindow *window);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // 0pos = enable/disabled cursor
    static unsigned int state = 0;

    if (action == GLFW_PRESS)
    {
        switch(key)
        {
            case GLFW_KEY_R:
            {
                //GS_Shader_RecompileProgram(globalShader);
                break;
            }
            case GLFW_KEY_ESCAPE:
            {
                // default is normal
                if (state & 1)
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                else
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                state ^= 1;

                break;
            }
        }
    }
    
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    static float lastX = 0, lastY = 0;
    float xoffset = (xpos - lastX) * sensitivity, 
            yoffset = (ypos - lastY) * sensitivity;
    
    lastX = xpos;
    lastY = ypos;

}


static void key_movement(GLFWwindow *window)
{
    static vec3 res;
    game_time.current = glfwGetTime();
    game_time.delta =  game_time.current - game_time.previous;
    game_time.previous = game_time.current;

    float speed = movementSpeed * game_time.delta;
    float Yspeed = 1.0f * game_time.delta;
};

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

}

#endif

#endif

/* 
    if (!glfwInit()) {
        GC_LOG("Failed to initialize GLFW.");
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        GC_LOG("Failed to create window.");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        GC_LOG("Failed to initialized GLAD.");
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glfwSetFramebufferSizeCallback(window, Window_FramebufferSizeCallback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
*/