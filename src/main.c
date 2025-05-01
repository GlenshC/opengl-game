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
#include "gc_material_const.h"
#include "gc_constants.h"
#include "main.h"

// #define GS_TEST_FACEBOX
// #include "test/gs_test.h"

//MY MACRO's PLACED HERE FOR NOW

typedef struct GS_Camera{
    vec3 position;
    vec3 front;
    vec3 direction;
    vec3 target;
    vec3 up;
    float fov;
    float pitch; // vertical
    float yaw; // horizontal
} GS_Camera;

typedef struct GS_Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
} GS_Material;

typedef struct GS_Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} GS_Light;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void Window_FramebufferSizeCallback(GLFWwindow *window, int width, int height);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
static void scroll_callback(GLFWwindow* window, double xpos, double ypos);
static void key_movement(GLFWwindow *window);

GLFWwindow* window;
GS_ShaderHandle globalShader;
float timeDelta;
float timePrev;
float timeCurr;

static float movementSpeed = 2.5f;
float sensitivity = 0.1;
static float CameraYPos = -0.4f;
static float clearColor[3] = {0.1f, 0.1f, 0.1f};
static float fontScale = 2.0f;


GS_Camera globalCamera = {
    {-4.0f, -0.4f, -9.0f},  // position
    {0.0f, -1.0f, 0.0f},    // front
    {0.0f, 0.0f, 0.0f},     // direction (facing)
    {0.0f, 0.0f, 0.0f},     // target (looking at)
    {0.0f, 1.0f, 0.0f},     // up
    70.0f,                  // fov
    0.0f,                   // pitch
    0.0f                    // yaw
};

GS_Material globalMaterial = {
    (float[3]) {1.0f, 0.5, 0.31f}, 
    (float[3]) {1.0f, 0.5, 0.31f},
    (float[3]) {0.5f, 0.5f, 0.5f},
    (float) 2.0f
};

GS_Light globalLight = {
    {.05f, -0.5f, -7.2f},
    {0.2f, 0.2f, 0.2f},
    {0.5f, 0.5f, 0.5f},
    {1.0f, 1.0f, 1.0f}
};

/* 
GS_Material globalMaterial = {
    (float[3]) {1.0f, 0.5, 0.31f},
    (float[3]) {1.0f, 0.5, 0.31f},
    (float[3]) {0.5f, 0.5f, 0.5f},
    (float) 1.0f
};
 */


int main(void)
{
    GS_MAIN_INIT;
    glfwSetScrollCallback(window, scroll_callback);

    // malloced shader;
    
    GS_ShaderHandle lightShader = GS_Shader_CreateProgram("./res/shaders/lightCube_vertex.glsl", "./res/shaders/lightCube_fragment.glsl");
    globalShader = GS_Shader_CreateProgram("./res/shaders/default_vertex.glsl", "./res/shaders/default_fragment.glsl");
    if(globalShader)
        GS_Shader_UseProgram(globalShader);

    GC_LOG("%u %u", lightShader, globalShader);
    
    /*ImGUI stuff*/
    igCreateContext(NULL);

    ImGuiIO *io = igGetIO();
    igStyleColorsDark(NULL);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    
    io->FontGlobalScale = fontScale;

    //DATA
    static const float vertices[] = GS_EXAMPLE_VERTEX_DATA;

    
    unsigned int v_array;
    glGenVertexArrays(1, &v_array);
    glBindVertexArray(v_array);
    
    unsigned int v_buffer;
    glGenBuffers(1, &v_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(*vertices)*8, (void *) (0) );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(*vertices)*8, (void *) (sizeof(float) * 3) );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(*vertices)*8, (void *) (sizeof(float) * 6) );
    glEnableVertexAttribArray(2);
    
    
    //GS_Shader_SetUniformVec3f(globalShader, "u_lightColor", lightColor);
    // GS_Shader_SetUniformVec3f(globalShader, "u_toyColor", toyColor);
    
    mat4 model = GLM_MAT4_IDENTITY_INIT, proj = GLM_MAT4_IDENTITY_INIT, view = GLM_MAT4_IDENTITY_INIT;
    vec3 boxPos =  {-1.05f, -1.3f, -3.26f};
    
    
    vec3 lightColor = {1.0f, 1.0f, 1.0f};
    vec3 diffuseColor;
    vec3 ambientColor;

    GSTexture diffuseMap = GS_GenTexture2D("./res/textures/container2.png", GL_RGBA);
    GSTexture specularMap = GS_GenTexture2D("./res/textures/container2_specular.png", GL_RGBA);
    GSTexture emissionMap = GS_GenTexture2D("./res/textures/matrix.jpg", GL_RGB);

    // GC_LOG("%u %u\n", lightShader, globalShader);

    // TODO implement movement
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        key_movement(window);

        lightColor[0] = sin(timeCurr * 2.0f);
        lightColor[1] = sin(timeCurr * 0.7f);
        lightColor[2] = sin(timeCurr * 1.3f);

        glm_vec3_mul(lightColor, (vec3){0.5f, 0.5f, 0.5f}, diffuseColor);
        glm_vec3_mul(diffuseColor, (vec3){0.2f, 0.2f, 0.2f}, ambientColor);
        // globalLight.position[0] = sin(timeCurr) * 10.0f;
        // globalLight.position[2] = cos(timeCurr) * 10.0f;

        // GL Render
        glm_vec3_add(globalCamera.position, globalCamera.front, globalCamera.target);
        glm_lookat(globalCamera.position, globalCamera.target, globalCamera.up, view);
        glm_perspective(glm_rad(globalCamera.fov), 1280.0f/720.0f, 0.1f, 100.0f, proj);

        GS_Shader_UseProgram(globalShader);
        glm_mat4_identity(model);
        glm_translate(model, boxPos);

        GS_ActiveTexture(0, GL_TEXTURE_2D, diffuseMap);
        GS_ActiveTexture(1, GL_TEXTURE_2D, specularMap);
        GS_ActiveTexture(2, GL_TEXTURE_2D, emissionMap);

        GS_Shader_SetUniformMat4(globalShader, "u_ModelMat", model);
        GS_Shader_SetUniformMat4(globalShader, "u_ViewMat", view);
        GS_Shader_SetUniformMat4(globalShader, "u_ProjMat", proj);
        GS_Shader_SetUniformVec3f(globalShader, "u_cameraPos", globalCamera.position);
        
        // GS_Shader_SetUniformVec3f(globalShader, "material.ambient", globalMaterial.ambient);
        GS_Shader_SetUniformInt(globalShader, "material.diffuse", 0);
        GS_Shader_SetUniformInt(globalShader, "material.specular", 1);
        GS_Shader_SetUniformInt(globalShader, "material.emission", 2);
        GS_Shader_SetUniformFloat(globalShader, "material.shininess", 64.0f);

        GS_Shader_SetUniformVec3f(globalShader, "u_lightPos", globalLight.position);
        GS_Shader_SetUniformVec3f(globalShader, "light.ambient", globalLight.ambient);
        GS_Shader_SetUniformVec3f(globalShader, "light.diffuse", globalLight.diffuse);
        GS_Shader_SetUniformVec3f(globalShader, "light.specular", globalLight.specular);
        
        glBindVertexArray(v_array);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        GS_Shader_UseProgram(lightShader);
        glm_mat4_identity(model);
        glm_translate(model, globalLight.position);

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
            
            igDragFloat3("light.ambient", globalLight.ambient, 0.001f, 0.0f, 1.0f, "%.3f", 0);
            igDragFloat3("light.diffuse", globalLight.diffuse, 0.001f, 0.0f, 1.0f, "%.3f", 0);
            igDragFloat3("light.specular", globalLight.specular, 0.001f, 0.0f, 1.0f, "%.3f", 0);

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
            igText("FOV: %.2f",
                   globalCamera.fov);

            igEnd();
        }
        
        igRender();
        io->FontGlobalScale = fontScale;    
        ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData()); // ImGUI Section END
         
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    igDestroyContext(NULL);

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
                    GS_Shader_RecompileProgram(globalShader);
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
    
        globalCamera.direction[0] = cos(glm_rad(globalCamera.yaw));
        globalCamera.front[0] = cos(glm_rad(globalCamera.yaw)) * cos(glm_rad(globalCamera.pitch));
        
        globalCamera.front[1] = -sin(glm_rad(globalCamera.pitch));

        globalCamera.direction[2] = sin(glm_rad(globalCamera.yaw));
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
    float Yspeed = 1.0f * timeDelta;

    if(glfwGetKey(window, GLFW_KEY_W))
    {
        glm_vec3_muladds(globalCamera.direction, speed, globalCamera.position);
    }
    
    if(glfwGetKey(window, GLFW_KEY_S))
    {
        glm_vec3_mulsubs(globalCamera.direction, speed, globalCamera.position);
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
    if(glfwGetKey(window, GLFW_KEY_SPACE))
    {
        CameraYPos += Yspeed;
        globalCamera.position[1] = CameraYPos;
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
    {
        CameraYPos -= Yspeed;
        globalCamera.position[1] = CameraYPos;
    }
};

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    globalCamera.fov += yoffset * 2.5f;
    if (globalCamera.fov > 120.0f)
        globalCamera.fov = 120.0f;
    if (globalCamera.fov < 30.0f)
        globalCamera.fov = 30.0f;
}


