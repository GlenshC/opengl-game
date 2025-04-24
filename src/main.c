#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui/cimgui.h"
#include "cimgui/cimgui_impl.h"
#include "cglm/cglm.h"

#include "shader.h"
#include "texture.h"
#include "gc_logs.h"

// #define GS_TEST_FACEBOX
// #include "test/gs_test.h"

//MY MACRO's PLACED HERE FOR NOW
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

GLFWwindow* window;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void Window_FramebufferSizeCallback(GLFWwindow *window, int width, int height);

typedef struct {
    float vData[3];
    float pos[3];
    float color[3];
} VertexData;


Shader *globalShader;
int main(void)
{
    if (!glfwInit()) {
        GC_LOG("Failed to initialize GLFW.");
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);
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
    

    // malloced shader;
    globalShader = GS_Shader_CreateProgram("./res/shaders/vertex.glsl", "./res/shaders/fragment.glsl");
    if(globalShader)
        GS_Shader_UseProgram(globalShader);
    
    /*ImGUI stuff*/
    igCreateContext(NULL);
    ImGuiIO *io = igGetIO();
    
    igStyleColorsDark(NULL);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    
    float clearColor[3] = {0.2f, 0.3f, 0.3f};
    float fontScale = 2.0f;
    io->FontGlobalScale = fontScale;         

    
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // GL Render
        
        // UI render
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        igNewFrame();
        
        {
            igBegin("Glensh App", NULL, 0);
            igColorEdit3("Clear Color",clearColor, 0);
            igSliderFloat("Font Scale", &fontScale, 1.0f, 4.0f, "%.1f", 0);
            igText("Application average %.3f ms/frame (%.1f FPS)",
                   1000.0f / igGetIO()->Framerate,
                   igGetIO()->Framerate);

            igEnd();
        }
        
        igRender();
        io->FontGlobalScale = fontScale;    
        ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData()); // ImGUI Section END
         
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }

    free(globalShader);
    glfwTerminate();
    return 0;
}

void Window_FramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);    
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
        GS_Shader_RecompileProgram(GS_Shader_GetActiveShader());
}