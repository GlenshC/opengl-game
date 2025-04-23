#include <stdlib.h>
#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "g_string.h"
#include "shader.h"
#include "glogs.h"

#define freeSources(x, y) free(x); free(y);
    

static int CompileShader(unsigned int renderID, const char* string);
static int LinkProgram(unsigned int renderID);

static char infoLog[1024];
static int status;

// For creation use only, not for recompiling;

Shader *GCShader_CreateShader(const char* vertexPath, const char* fragmentPath)
{
    GLOGS_LOG("Creating Shader...\n");
    Shader *shader;
    unsigned int vID, fID;

    // impl string copy..
    
    vID = glCreateShader(GL_VERTEX_SHADER);
    fID = glCreateShader(GL_FRAGMENT_SHADER);

    G_String *vSource = G_ReadFile(vertexPath);
    G_String *fSource = G_ReadFile(fragmentPath);
    if (!vSource || !fSource)
    {
        GLOGS_LOG("Failed to open Shader Source.\n");
        
        return NULL;
    }
    
    if (CompileShader(vID, vSource->string) || CompileShader(fID, fSource->string))
    {
        free(vSource);        
        free(fSource);        
        return NULL;
    }

    free(vSource);        
    free(fSource);        
    
    shader = (Shader *)malloc(sizeof(Shader));
    
    shader->renderID = glCreateProgram();

    glAttachShader(shader->renderID, vID);
    glAttachShader(shader->renderID, fID);

    unsigned int linkStatus = LinkProgram(shader->renderID);
    if (linkStatus);
    {
        shader->status = -1;
        return shader;
    }

    GLOGS_LOG("Shader Created Succesfully!\n\n");
    shader->status = 0;
    
    return shader;
}

void GCShader_UseShader(Shader *shader)
{
    if (shader->renderID != -1)
    {
        GLOGS_LOG("Shader Active.\n");
        glUseProgram(shader->renderID);
    }
    else
        GLOGS_LOG("UseShader(Shader *shader):\n"
               "Shader cannot be used,"
               "an error occured while compiling and linking shader\n\n");
    
}


void GCShader_SetUniformMat4(Shader *shader, const char *name, void * mat4)
{
    int location = glGetUniformLocation(shader->renderID, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, mat4);
}


/*
    Private Help Functions
 */

static int CompileShader(unsigned int renderID, const char *source)
{
    GLOGS_LOG("Compiling Shader...\n");
    int logLen;
    glShaderSource(renderID, 1, &source, NULL);
    glCompileShader(renderID);
    
    glGetShaderiv(renderID, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        GLOGS_LOG("Compile Status: Error\n");
        glGetShaderInfoLog(renderID, 1024, &logLen, infoLog);
        GLOGS_LOG("Shader Compilation Error:\n%s",infoLog);
        
        return 1;
    }

    GLOGS_LOG("Successfully compiled shader!\n");
    return 0;
}

static int LinkProgram(unsigned int renderID)
{
    unsigned int logLen;
    glLinkProgram(renderID);
    glGetProgramiv(renderID, GL_LINK_STATUS, &status);
    if (!status)
    {
        glGetProgramInfoLog(renderID, 1024, &logLen, infoLog);
        GLOGS_LOG("Shader Program Linking Error:\n%s",infoLog);
        
        return 1;
    }
    GLOGS_LOG("Successfully linked shader program!\n");
    return 0;
}
    
