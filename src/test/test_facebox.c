#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui/cimgui.h"
#include "cimgui/cimgui_impl.h"
#include "cglm/cglm.h"

#include "shader.h"
#include "texture.h"
#include "gc_logs.h"
#include "gs_test.h"

static mat4 
    modelMat = GLM_MAT4_IDENTITY_INIT, 
    viewMat  = GLM_MAT4_IDENTITY_INIT, 
    projMat  = GLM_MAT4_IDENTITY_INIT;

static Shader *shader;
static unsigned int vertex_array;
static unsigned int vertex_buffer;
static GSTexture faceTexture;
static GSTexture boxTexture;
static float scales;
static vec3 rotation;
static vec3 transVec;

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

static vec3 cubePositions[] = {
    { 0.0f,  0.0f,   0.0f},
    { 2.0f,  5.0f, -15.0f},
    {-1.5f, -2.2f,  -2.5f},
    {-3.8f, -2.0f, -12.3f},
    { 2.4f, -0.4f,  -3.5f},
    {-1.7f,  3.0f,  -7.5f},
    { 1.3f, -2.0f,  -2.5f},
    { 1.5f,  2.0f,  -2.5f},
    { 1.5f,  0.2f,  -1.5f},
    {-1.3f,  1.0f,  -1.5f}
};


void GS_TEST_FaceBox_Init()
{
    shader = GS_Shader_CreateProgram("./res/shaders/facebox_vertex.glsl", "./res/shaders/facebox_fragment.glsl");
    if(shader)
        GS_Shader_UseProgram(shader);

    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
                          sizeof(*vertices) * 5, (void *) 0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
        sizeof(*vertices) * 5, (void *) (sizeof(float)*3));

    // Texture stuff

    faceTexture = GS_GenTexture2D("./res/textures/awesomeface.png", GL_RGBA);
    boxTexture = GS_GenTexture2D("./res/textures/container.jpg", GL_RGB);

    // Transform stuff
    scales = 1.0f;

    glm_perspective(glm_rad(90.0f), 1280.0f/720.0f, 0.1f, 100.0f, projMat);
}

void GS_TEST_FaceBox_Update()
{
    GS_Shader_UseProgram(shader);
    GS_Shader_SetInt(shader, "texture0", 0);
    GS_Shader_SetInt(shader, "texture1", 1);
    GS_ActiveTexture(0, GL_TEXTURE_2D, boxTexture);    
    GS_ActiveTexture(1, GL_TEXTURE_2D, faceTexture);

    glm_mat4_identity(modelMat);
    glm_translate(modelMat, transVec);
    glm_rotate(modelMat, glm_rad(rotation[0]), (vec3){1.0f, 0.0f, 0.0f});
    glm_rotate(modelMat, glm_rad(rotation[1]), (vec3){0.0f, 1.0f, 0.0f});
    glm_rotate(modelMat, glm_rad(rotation[2]), (vec3){0.0f, 0.0f, 1.0f});
    glm_scale_uni(modelMat, scales);

    GS_Shader_SetUniformMat4(shader, "u_ModelMat", modelMat);
    GS_Shader_SetUniformMat4(shader, "u_ProjMat", projMat);

    glBindVertexArray(vertex_array);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void GS_TEST_FaceBox_GUIUpdate()
{
    igBegin("FaceBox Test", NULL, 0);
    igDragFloat("Scale", &scales, 0.01f, -10.0f, 10.0f, "%.3f", 0);
    igDragFloat3("Rotation", rotation, 0.01f, -360.0f, 360.0f, "%.2f", 0);
    igDragFloat3("Translate", transVec, 0.1f, -1000.0f, 10.0f, "%.3f", 0);
    igEnd();
}

void GS_TEST_FaceBox_End()
{
    glDeleteProgram(shader->renderID);
    free(shader);
}