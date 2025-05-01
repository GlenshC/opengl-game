#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "cglm/cglm.h"

#include "gc_logs.h"
#include "shader.h"
#include "texture.h"
#include "breakout.h"

typedef struct 
{
    int sample;
} GAME_STATE;

/* Global Variables */
static GS_ShaderHandle mainShader;
static mat4 projMat, modelMat;
static unsigned int quadVAO;
static unsigned int VBO;

static GSTextureHandle ballTexture;
static GSTextureHandle bgTexture;
static GSTextureHandle paddleTexture;
static GSTextureHandle blockTexture;
static GSTextureHandle solidTexture;

static vec3 spriteColor = {1.0f, 0.0f, 0.0f};
static vec3 ballPosition = {200.0f, 200.0f, 0.0f};
static vec3 ballSize = {200.0f, 200.0f, 0.0f};

/* Functions */
static void Breakout_Events(void);

void Breakout_Init(void)
{
    mainShader = GS_Shader_CreateProgram("./res/shaders/breakout/vertex.glsl","./res/shaders/breakout/fragment.glsl");
    ballTexture = GS_GenTexture2D("./res/textures/awesomeface.png", 1);
    bgTexture = GS_GenTexture2D("./res/textures/background.jpg", 0);
    paddleTexture = GS_GenTexture2D("./res/textures/paddle.png", 1);
    blockTexture = GS_GenTexture2D("./res/textures/block.png", 1);
    solidTexture = GS_GenTexture2D("./res/textures/block_solid.png", 1);

    float vertices[] = { 
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);

    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0 , 4, GL_FLOAT, GL_FALSE, sizeof(float)*4, (void *) (0));
    
    glBindVertexArray(0);

    glm_ortho(0.0f, 1280.f, 720.0f, 0.0f, -1.0f, 1.0f, projMat);
    glm_mat4_identity(modelMat);
}


void Breakout_Update(void)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    Breakout_Events();
    
    GS_Shader_UseProgram(mainShader);
    GS_ActiveTexture(0, GL_TEXTURE_2D, ballTexture);

    glm_mat4_identity(modelMat);
    glm_translate(modelMat, ballPosition);
    
    glm_translate(modelMat, (vec3){200.0f, 200.0f, 0.0f});
    glm_rotate(modelMat, glm_rad(45.0f), (vec3){0,0,1.0f});
    glm_translate(modelMat, (vec3){-200.0f, -200.0f, 0.0f});
    
    glm_scale_uni(modelMat, 400.0f);

    GS_Shader_SetUniformInt(mainShader, "image", 0);

    GS_Shader_SetUniformVec3f(mainShader, "spriteColor", spriteColor);

    GS_Shader_SetUniformMat4(mainShader, "u_ProjMat", projMat);
    GS_Shader_SetUniformMat4(mainShader, "u_ModelMat", modelMat);

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

static void Breakout_Events(void)
{

}