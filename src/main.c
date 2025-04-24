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
    

//  v-sync
//  glfwSwapInterval(1);

    // malloced shader;
    globalShader = GS_Shader_CreateProgram("./res/shaders/vertex.glsl", "./res/shaders/fragment.glsl");
    if(globalShader)
        GS_Shader_UseProgram(globalShader);
    
     const float vertices[] = {
        //object vertices     //texCoord
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    vec3 cubePositions[] = {
        { 0.0f,  0.0f,  0.0f},
        { 2.0f,  5.0f, -15.0f},
        {-1.5f, -2.2f, -2.5f},
        {-3.8f, -2.0f, -12.3f},
        { 2.4f, -0.4f, -3.5f},
        {-1.7f,  3.0f, -7.5f},
        { 1.3f, -2.0f, -2.5f},
        { 1.5f,  2.0f, -2.5f},
        { 1.5f,  0.2f, -1.5f},
        {-1.3f,  1.0f, -1.5f}
    };
    
    unsigned int vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    
    unsigned int vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
                          sizeof(*vertices) * 5, (void *) 0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          sizeof(*vertices) * 5, (void *) (sizeof(float)*3));
    
/*
    unsigned int index_buffer;
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);    
*/

    mat4 orthoProj = GLM_MAT4_IDENTITY_INIT;
    glm_ortho(0.0f, 1280.0f, 720.0f, 0.0f, -1.0f, 2.0f, orthoProj);

    GS_Shader_UseProgram(globalShader);
    GS_Shader_SetUniformMat4(globalShader, "u_Proj", orthoProj);

    // Texture stuff

    GSTexture faceTexture = GS_GenTexture2D("./res/textures/awesomeface.png", GL_RGBA);
    GSTexture boxTexture = GS_GenTexture2D("./res/textures/container.jpg", GL_RGB);

    // Transform stuff

    mat4 modelMat = GLM_MAT4_IDENTITY_INIT, 
         viewMat  = GLM_MAT4_IDENTITY_INIT, 
         projMat  = GLM_MAT4_IDENTITY_INIT;

    float scales = 1.0f;
    vec3 rotation = {};
    vec3 transVec = {};

    glm_perspective(glm_rad(90.0f), 1280.0f/720.0f, 0.1f, 100.0f, projMat);
    
    
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
        GS_Shader_SetInt(globalShader, "texture0", 0);
        GS_Shader_SetInt(globalShader, "texture1", 1);
        GS_ActiveTexture(0, GL_TEXTURE_2D, boxTexture);    
        GS_ActiveTexture(1, GL_TEXTURE_2D, faceTexture);

        glm_mat4_identity(modelMat);
        glm_translate(modelMat, transVec);
        glm_rotate(modelMat, glm_rad(rotation[0]), (vec3){1.0f, 0.0f, 0.0f});
        glm_rotate(modelMat, glm_rad(rotation[1]), (vec3){0.0f, 1.0f, 0.0f});
        glm_rotate(modelMat, glm_rad(rotation[2]), (vec3){0.0f, 0.0f, 1.0f});
        glm_scale(modelMat, (vec3){scales, scales, scales});
    
        GS_Shader_SetUniformMat4(globalShader, "u_ModelMat", modelMat);
        GS_Shader_SetUniformMat4(globalShader, "u_ProjMat", projMat);
        // GS_Shader_SetUniformMat4(globalShader, "u_ViewMat", viewMat);
            
        //glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void *)0); 
        glBindVertexArray(vertex_array);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // UI render
        // ImGUI Section START

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        igNewFrame();

        {
            igBegin("Glensh App", NULL, 0);
            igColorEdit3("Clear Color",clearColor, 0);
            igSliderFloat("Font Scale", &fontScale, 1.0f, 4.0f, "%.1f", 0);
            igDragFloat("Scale", &scales, 0.01f, -10.0f, 10.0f, "%.3f", 0);
            igDragFloat3("Rotation", rotation, 0.01f, -360.0f, 360.0f, "%.2f", 0);
            igDragFloat3("Translate", transVec, 0.1f, -1000.0f, 10.0f, "%.3f", 0);
            //igDragFloat2("Blue Box", vertices[0].pos, 1.0f, 0.0f, 1080.0f, "%.1f", 0);
            //igDragFloat2("Green Box", vertices[4].pos, 1.0f, 0.0f, 1080.0f, "%.1f", 0); 
            igText("Application average %.3f ms/frame (%.1f FPS)",
                   1000.0f / igGetIO()->Framerate,
                   igGetIO()->Framerate);

            igEnd();
        }
        
        igRender();
        io->FontGlobalScale = fontScale;    
        ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData()); // ImGUI Section END
/*
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
*/                
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