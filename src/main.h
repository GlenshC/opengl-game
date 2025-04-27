#ifndef GS_MAIN_H
#define GS_MAIN_H

#define GS_EXAMPLE_VERTEX_DATA {              \
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, \
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, \
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, \
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, \
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, \
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, \
                                              \
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, \
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, \
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, \
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, \
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, \
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, \
                                              \
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, \
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, \
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, \
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, \
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, \
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, \
                                              \
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, \
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, \
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, \
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, \
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, \
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, \
                                              \
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, \
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, \
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, \
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, \
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, \
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, \
                                              \
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, \
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, \
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, \
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, \
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, \
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f  \
}

#ifndef offsetof
#   define offsetof(TYPE, ELEMENT) ((size_t) &(((TYPE *)0)->ELEMENT))
#   ifdef __has_builtin
#       if __has_builtin(__builtin_offsetof)
#           undef offsetof
#           define offsetof(TYPE, MEMBER) __builtin_offsetof(TYPE, MEMBER)
#       endif
#   endif
#endif

#define igGetIO igGetIO_Nil
#define W_WIDTH 1280
#define W_HEIGHT 720


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
    }                                                                       \
                                                                            \
    glEnable(GL_DEPTH_TEST);                                                \
    glfwSetFramebufferSizeCallback(window, Window_FramebufferSizeCallback); \
    glfwSetKeyCallback(window, key_callback);                               \
    glfwSetCursorPosCallback(window, cursor_position_callback)

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