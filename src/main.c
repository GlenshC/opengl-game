#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui/cimgui.h"
#include "cimgui/cimgui_impl.h"
#include "cglm/cglm.h"

#include "shader.h"
#include "glogs.h"

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
        GLOGS_LOG("Failed to initialize GLFW.");
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);
    if (!window)
    {
        GLOGS_LOG("Failed to create window.");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        GLOGS_LOG("Failed to initialized GLAD.");
        glfwTerminate();
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, Window_FramebufferSizeCallback);

//  v-sync
//  glfwSwapInterval(1);

    // malloced shader;
    globalShader = GCShader_CreateShader("./res/shaders/vertex.glsl", "./res/shaders/fragment.glsl");
    if(globalShader)
        GCShader_UseShader(globalShader);
    
    VertexData vertices[] = {
        { {   0.0f,   0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f} },
        { {   0.0f, 200.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f} },
        { { 200.0f, 200.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f} },
        { { 200.0f,   0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f} },

        { {   0.0f,   0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} },
        { { 200.0f,   0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} },
        { { 200.0f, 200.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} },
        { {   0.0f, 200.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} }
    };

    unsigned int indices[] = {
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7,
    };

    unsigned int vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    
    unsigned int vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          sizeof(*vertices), (void *) offsetof(VertexData, vData));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          sizeof(*vertices), (void *) offsetof(VertexData, pos));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
                          sizeof(*vertices), (void *) offsetof(VertexData, color));

    unsigned int index_buffer;
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);    
// 1280, 720
    mat4 orthoProj = GLM_MAT4_IDENTITY_INIT;
    glm_ortho(0.0f, 1280.0f, 720.0f, 0.0f, -1.0f, 2.0f, orthoProj);

    GCShader_UseShader(globalShader);
    GCShader_SetUniformMat4(globalShader, "u_Proj", orthoProj);
    
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
        glClear(GL_COLOR_BUFFER_BIT);

        // GL Render
        
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void *)0); 

        // UI render
        // ImGUI Section START

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        igNewFrame();

        {
            igBegin("Glensh App", NULL, 0);
            igColorEdit3("Clear Color",clearColor, 0);
            igSliderFloat("Font Scale", &fontScale, 1.0f, 4.0f, "%.1f", 0);
            igDragFloat2("Blue Box", vertices[0].pos, 1.0f, 0.0f, 1080.0f, "%.1f", 0);
            igDragFloat2("Green Box", vertices[4].pos, 1.0f, 0.0f, 1080.0f, "%.1f", 0); 
            igText("Application average %.3f ms/frame (%.1f FPS)",
                   1000.0f / igGetIO()->Framerate,
                   igGetIO()->Framerate);

            igEnd();
        }
        
        igRender();
        io->FontGlobalScale = fontScale;    
        ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData()); // ImGUI Section END

        for (int i = 0; i < 2; i++)
        {
            int x = vertices[i*4].pos[0],
                y = vertices[i*4].pos[1];
            if (y > 520.0f)
            {
                y = 520.0f;
                vertices[i*4].pos[1] = y;
            }   
            for (int j = 1; j < 4; j++)
            {
                vertices[i*4 + j].pos[0] = x;
                vertices[i*4 + j].pos[1] = y;
                
            }
        }
        
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);        
                
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
