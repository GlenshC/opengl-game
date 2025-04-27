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
#define W_WIDTH 1280
#define W_HEIGHT 720

GLFWwindow* window;
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void Window_FramebufferSizeCallback(GLFWwindow *window, int width, int height);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
static void key_movement(GLFWwindow *window);

typedef struct {
    float vData[3];
    float pos[3];
    float color[3];
} VertexData;

typedef struct {
    vec3 position;
    vec3 front;
    vec3 target;
    vec3 up;
    float pitch; // vertical
    float yaw; // horizontal
} GS_Camera;

GS_Camera globalCamera;
float timeDelta;
float timePrev;
float timeCurr;

static float movementSpeed = 2.5f;
float sensitivity = 0.1;

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

    // malloced shader;
    
    Shader *lightShader = GS_Shader_CreateProgram("./res/shaders/vertex.glsl", "./res/shaders/lightCube_fragment.glsl");
    globalShader = GS_Shader_CreateProgram("./res/shaders/vertex.glsl", "./res/shaders/fragment.glsl");
    if(globalShader)
        GS_Shader_UseProgram(globalShader);
    
    /*ImGUI stuff*/
    float clearColor[3] = {0.1f, 0.1f, 0.1f};
    float fontScale = 2.0f;

    igCreateContext(NULL);

    ImGuiIO *io = igGetIO();
    igStyleColorsDark(NULL);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    
    io->FontGlobalScale = fontScale;

    //DATA
    static const float vertices[] = {
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

    
    unsigned int v_array;
    glGenVertexArrays(1, &v_array);
    glBindVertexArray(v_array);
    
    unsigned int v_buffer;
    glGenBuffers(1, &v_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(*vertices)*5, (void *) (0) );
    
    vec3 lightColor = {1.0f, 1.0f, 1.0f};
    vec3 toyColor = {1.0f, 0.5f, 0.31f};
    
    GS_Shader_SetUniformVec3f(globalShader, "u_lightColor", lightColor);
    GS_Shader_SetUniformVec3f(globalShader, "u_toyColor", toyColor);

    mat4 model = GLM_MAT4_IDENTITY_INIT, proj = GLM_MAT4_IDENTITY_INIT, view = GLM_MAT4_IDENTITY_INIT;
    
    vec3 lightPos =  {.05f, -0.5f, -7.2f};
    vec3 boxPos =  {-1.05f, -1.3f, -3.26f};
    
    vec3 camTarget = {};
    globalCamera.position[1] = 3.0f;
    globalCamera.position[2] = 3.0f;
	globalCamera.front[1] = -0.5f;
	globalCamera.front[1] = -1.0f;
	globalCamera.up[1] = 1.0f;

    glm_perspective(glm_rad(45.0f), 1280.0f/720.0f, 0.1f, 100.0f, proj);
    globalCamera.pitch = 7.2f;
    globalCamera.yaw = 240.0f;
    globalCamera.position[0] = 1.5f;
    globalCamera.position[1] = -0.4f;
    globalCamera.position[2] = 2.5f;

    // TODO implement movement
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        key_movement(window);

        // GL Render
        glm_vec3_add(globalCamera.position, globalCamera.front, globalCamera.target);
        glm_lookat(globalCamera.position, globalCamera.target, globalCamera.up, view);

        GS_Shader_UseProgram(globalShader);
        glm_mat4_identity(model);
        glm_translate(model, boxPos);
        GS_Shader_SetUniformMat4(globalShader, "u_ModelMat", model);
        GS_Shader_SetUniformMat4(globalShader, "u_ViewMat", view);
        GS_Shader_SetUniformMat4(globalShader, "u_ProjMat", proj);
        
        glBindVertexArray(v_array);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        
        GS_Shader_UseProgram(lightShader);
        glm_mat4_identity(model);
        glm_translate(model, lightPos);

        GS_Shader_SetUniformMat4(lightShader, "u_ModelMat", model);
        GS_Shader_SetUniformMat4(lightShader, "u_ViewMat", view);
        GS_Shader_SetUniformMat4(lightShader, "u_ProjMat", proj);

        glBindVertexArray(v_array);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        // UI render
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        igNewFrame();
        
        {
            igBegin("Glensh App", NULL, 0);
            igColorEdit3("Clear Color",clearColor, 0);
            igSliderFloat("Font Scale", &fontScale, 1.0f, 4.0f, "%.1f", 0);
            igSliderFloat("Movement Speed", &movementSpeed, 1.0f, 4.0f, "%.1f", 0);
            igDragFloat3("Box", boxPos, 0.01f, -100.0f, 10.0f, "%.2f", 0);
            igText("Application average %.3f ms/frame (%.1f FPS)",
                   1000.0f / igGetIO()->Framerate,
                   igGetIO()->Framerate);
            igText("Pitch: %.2f, Yaw: %.2f",
                   globalCamera.pitch,
                   globalCamera.yaw);
            igText("Position: %.2f, %.2f, %.2f",
                   globalCamera.position[0],
                   globalCamera.position[1],
                   globalCamera.position[2]);

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
    // 0pos = enable/disabled cursor
    static int state = 0;
    

    switch(action)
    {
        case GLFW_PRESS:
        {
            switch(key)
            {
                case GLFW_KEY_R:
                {
                    GS_Shader_RecompileProgram(GS_Shader_GetActiveShader());
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
            break;
        }

        default:
            break;
    }
    
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    static float lastX = 0, lastY = 0;
    float xoffset = (xpos - lastX) * sensitivity, 
          yoffset = (ypos - lastY) * sensitivity;
    
    lastX = xpos;
    lastY = ypos;

    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
    {
    
        globalCamera.yaw += xoffset;  
        globalCamera.pitch += yoffset;  
    
        if (globalCamera.pitch > 89.99f)
            globalCamera.pitch = 89.99f;
        if (globalCamera.pitch < -89.99f)
            globalCamera.pitch = -89.99f;
    
        globalCamera.front[0] = cos(glm_rad(globalCamera.yaw)) * cos(glm_rad(globalCamera.pitch));
        globalCamera.front[1] = -sin(glm_rad(globalCamera.pitch));
        globalCamera.front[2] = sin(glm_rad(globalCamera.yaw)) * cos(glm_rad(globalCamera.pitch)); 
    
        // printf("pitch: %.2f\nyaw: %.2f\n",globalCamera.pitch, globalCamera.yaw);
        glm_normalize(globalCamera.front);

    }


}


static void key_movement(GLFWwindow *window)
{
    
    static vec3 res;
    timeCurr = glfwGetTime();
    timeDelta =  timeCurr - timePrev;
    timePrev = timeCurr;

    float speed = movementSpeed * timeDelta;

    if(glfwGetKey(window, GLFW_KEY_W))
    {
        glm_vec3_muladds(globalCamera.front, speed, globalCamera.position);
    }
    
    if(glfwGetKey(window, GLFW_KEY_S))
    {
        glm_vec3_mulsubs(globalCamera.front, speed, globalCamera.position);
        // MOVE
    }

    if(glfwGetKey(window, GLFW_KEY_A))
    {
        // MOVE
        glm_vec3_crossn(globalCamera.front, globalCamera.up, res);
        glm_vec3_mulsubs(res, speed, globalCamera.position);
    }
    
    if(glfwGetKey(window, GLFW_KEY_D))
    {
        glm_vec3_crossn(globalCamera.front, globalCamera.up, res);
        glm_vec3_muladds(res, speed, globalCamera.position);
        // MOVE
    }
};

