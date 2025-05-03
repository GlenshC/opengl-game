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
#include "gc_example_data.h"
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
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
} GS_Light;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void Window_FramebufferSizeCallback(GLFWwindow *window, int width, int height);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
static void scroll_callback(GLFWwindow* window, double xpos, double ypos);
static void key_movement(GLFWwindow *window);

GLFWwindow* window;
GS_ShaderHandle globalShader;

GS_Time game_time;

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
    {1.2f, 1.0f, 2.0f},
    {-0.2f, -1.0f, -0.3f},
    {0.2f, 0.2f, 0.2f},
    {0.5f, 0.5f, 0.5f},
    {1.0f, 1.0f, 1.0f},
    1.0f, 0.09f, 0.032f
};


int main(void)
{
    window = GS_GLFW_INIT();
    if (window == NULL)
        return -1;
    glEnable(GL_DEPTH_TEST);
    glfwSetFramebufferSizeCallback(window, Window_FramebufferSizeCallback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // malloced shader;
    
    GS_ShaderHandle lightShader = GS_Shader_CreateProgram("./res/shaders/lightCube_vertex.glsl", "./res/shaders/lightCube_fragment.glsl");
    globalShader = GS_Shader_CreateProgram("./res/shaders/default_vertex.glsl", "./res/shaders/default_fragment.glsl");
    if(globalShader)
        GS_Shader_UseProgram(globalShader);
    
    /*ImGUI stuff*/
    igCreateContext(NULL);

    ImGuiIO *io = igGetIO();
    igStyleColorsDark(NULL);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    
    io->FontGlobalScale = fontScale;

    //DATA
    static const float vertices[] = GS_EXAMPLE_DATA_CUBE;
    static const float cubePos[] = GS_DATA_CUBE_POSITIONS;

    
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
    
    mat4 model = GLM_MAT4_IDENTITY_INIT, proj = GLM_MAT4_IDENTITY_INIT, view = GLM_MAT4_IDENTITY_INIT;
    vec3 boxPos =  {-1.05f, -1.3f, -3.26f};
    
    GSTextureHandle diffuseMap = GS_GenTexture2D("./res/textures/container2.png", 1);
    GSTextureHandle specularMap = GS_GenTexture2D("./res/textures/container2_specular.png", 1);
    GSTextureHandle emissionMap = GS_GenTexture2D("./res/textures/matrix.jpg", 0);

    
    // TODO implement movement
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        key_movement(window);

        // GL Render
        glm_vec3_add(globalCamera.position, globalCamera.front, globalCamera.target);
        glm_lookat(globalCamera.position, globalCamera.target, globalCamera.up, view);
        glm_perspective(glm_rad(globalCamera.fov), 1280.0f/720.0f, 0.1f, 100.0f, proj);

        GS_Shader_UseProgram(globalShader);
        glm_mat4_identity(model);
        glm_translate(model, boxPos);

        GS_ActiveTexture(0, GL_TEXTURE_2D, diffuseMap);
        GS_ActiveTexture(1, GL_TEXTURE_2D, specularMap);
        //GS_ActiveTexture(2, GL_TEXTURE_2D, emissionMap);

        GS_Shader_SetUniformMat4(globalShader, "u_ViewMat", view);
        GS_Shader_SetUniformMat4(globalShader, "u_ProjMat", proj);
        GS_Shader_SetUniformVec3f(globalShader, "u_cameraPos", globalCamera.position);
        
        // GS_Shader_SetUniformVec3f(globalShader, "material.ambient", globalMaterial.ambient);
        GS_Shader_SetUniformInt(globalShader, "material.diffuse", 0);
        GS_Shader_SetUniformInt(globalShader, "material.specular", 1);
        //GS_Shader_SetUniformInt(globalShader, "material.emission", 2);
        GS_Shader_SetUniformFloat(globalShader, "material.shininess", 64.0f);
        
        // GS_Shader_SetUniformVec3f(globalShader, "light.position", globalLight.position);
        // GS_Shader_SetUniformVec3f(globalShader, "light.direction", globalLight.direction);
        GS_Shader_SetUniformVec3f(globalShader, "light.position", globalCamera.position);
        GS_Shader_SetUniformVec3f(globalShader, "light.direction", globalCamera.front);
        GS_Shader_SetUniformFloat(globalShader, "light.cutoff", cos(glm_rad(30.5f)));


        GS_Shader_SetUniformVec3f(globalShader, "light.ambient", globalLight.ambient);
        GS_Shader_SetUniformVec3f(globalShader, "light.diffuse", globalLight.diffuse);
        GS_Shader_SetUniformVec3f(globalShader, "light.specular", globalLight.specular);
        GS_Shader_SetUniformFloat(globalShader, "light.constant", globalLight.constant);
        GS_Shader_SetUniformFloat(globalShader, "light.linear", globalLight.linear);
        GS_Shader_SetUniformFloat(globalShader, "light.quadratic", globalLight.quadratic);
        
        glBindVertexArray(v_array);
        for (int i =0; i < 10; i++)
        {
            glm_mat4_identity(model);
            glm_translate(model, &cubePos[i*3]);
            float angle = 20.0f * i;
            glm_rotate(model, glm_rad(angle), (vec3) {1.0f, 0.3f, 0.5f});
            GS_Shader_SetUniformMat4(globalShader, "u_ModelMat", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        
        // GS_Shader_UseProgram(lightShader);
        // glm_mat4_identity(model);
        // glm_translate(model, globalLight.position);
        // glm_scale_uni(model, 0.2f);
        // GS_Shader_SetUniformMat4(lightShader, "u_ModelMat", model);
        // GS_Shader_SetUniformMat4(lightShader, "u_ViewMat", view);
        // GS_Shader_SetUniformMat4(lightShader, "u_ProjMat", proj);

        // glBindVertexArray(v_array);
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        
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
                    GS_Shader_RecompileAll();
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
    game_time.current = glfwGetTime();
    game_time.delta =  game_time.current - game_time.previous;
    game_time.previous = game_time.current;

    float speed = movementSpeed * game_time.delta;
    float Yspeed = 1.0f * game_time.delta;

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