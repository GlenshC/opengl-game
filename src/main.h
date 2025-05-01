
#ifndef GS_MAIN_H
#define GS_MAIN_H

#define W_WIDTH 1280
#define W_HEIGHT 720

typedef struct {
    float delta,
          previous,
          current;
} GS_Time;


void Window_FramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);    
}

GLFWwindow *GS_GLFW_INIT()
{
    if (!glfwInit()) {
        GC_LOG("Failed to initialize GLFW.");
        return NULL;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow *window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        GC_LOG("Failed to create window.");
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        GC_LOG("Failed to initialized GLAD.");
        glfwTerminate();
        return NULL;
    }

    return window;
}

#endif