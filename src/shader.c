#include <stdlib.h>
#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "gc_string.h"
#include "gc_logs.h"

#define freeSources(x, y) free(x); free(y);
// typedef struct {
//     unsigned int
//         vertexID,
//         fragmentID;
//     GC_String 
//         *vSource;
//         *fSource;
// } ShaderProgram_Shaders;

/* 
GS_Shader {
    renderID;
}
GS_ShaderSourcePath {
    vertexPath;
    fragmentPath;
}
ShaderPrograms = {
    GS_Shader : GS_ShaderSourcePath 
    GS_Shader : GS_ShaderSourcePath 
    GS_Shader : GS_ShaderSourcePath 
    GS_Shader : GS_ShaderSourcePath 
}

recompiling the shader
we want a recompile function
where if we give a in use shader it should preserve it if there is an error
*/
    

static void GS_CompileProgramShaders(Shader *shader);
static int GS_CompileShader(GS_ShaderID *renderID, const char *path, GLenum type);
static GS_ProgramID GS_AttachShader(GS_ShaderID vID, GS_ShaderID fID);
static int GS_LinkProgram(GS_ProgramID renderID);
static int GS_Shader_IsActive(GS_ProgramID renderID);

static char infoLog[1024];
static GS_ProgramID GS_ActiveProgram;
static Shader *GS_ActiveProgramPointer;

/*
    Returns 1 if the shader is currently in use;
*/

/*
    Recompiles shader program. 
    if recompilation fails it preserve the previous working renderID,
    else it stores the new id to renderID, and deletes the old one; 
*/
void GS_Shader_RecompileProgram(Shader *shader)
{
    unsigned int renderID = shader->renderID;
    GC_LOG("Recompiling Shader...\n");

    GS_CompileProgramShaders(shader);
    if (shader->renderID == 0) {
    
        GC_LOG("Failed to recompile shader program.\n\n");
        shader->renderID = renderID;
    
    } else {
        GC_LOG("Successfully recompiled shader program.\n\n");
        
        if (GS_Shader_IsActive(renderID)) 
        {
            GS_Shader_UseProgram(shader);
        }
        
        glDeleteProgram(renderID);
    }
}

/* 
    Mallocs a shader program struct
    returns NULL if memory allocation fails;
    if Shader->renderID is 0 then a shader compiling/linking error occured
*/
Shader *GS_Shader_CreateProgram(const char* vertexPath, const char* fragmentPath)
{
    GC_LOG("Creating Shader...\n");
    Shader *shader = (Shader *)malloc(sizeof(Shader));
    if (shader == NULL)
    {
        return NULL;
    }

    GC_strcpy(shader->vertexPath, vertexPath, 0, 512);
    GC_strcpy(shader->fragmentPath, fragmentPath, 0, 512);
    
    GS_CompileProgramShaders(shader);

    GC_LOG("\nShader Created Succesfully!\n\n");
    
    return shader;
}

void GS_Shader_UseProgram(Shader *shader)
{
    if (shader->renderID == 0)
    {
        // GC_LOG("UseShader(Shader *shader):\n"
        //        "Shader cannot be used,"
        //        "an error occured while create shader.\n\n");
        return;
    }
    //GC_LOG("Shader Active.\n\n");
    glUseProgram(shader->renderID);
    GS_ActiveProgram = shader->renderID;
    GS_ActiveProgramPointer = shader;
}

Shader *GS_Shader_GetActiveShader()
{
    if (GS_ActiveProgramPointer == NULL)
        return NULL;
    return GS_ActiveProgramPointer;
}

void GS_Shader_SetUniformMat4(Shader *shader, const char *name, void * Mat4)
{
    int location = glGetUniformLocation(shader->renderID, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, Mat4);
}

void GS_Shader_SetUniformVec3f(Shader *shader, const char *name, void *Vec3)
{
    int location = glGetUniformLocation(shader->renderID, name);
    glUniform3fv(location, 1, Vec3);
}

void GS_Shader_SetUniformInt(Shader *shader, const char *name, int value)
{
    int location = glGetUniformLocation(shader->renderID, name);
    glUniform1i(location, value);
}




/* 
    PRIVATE FUNCTIONS
*/

static int GS_Shader_IsActive(GS_ProgramID renderID)
{
    if (renderID == GS_ActiveProgram)
        return 1;
    return 0;
}

/*
Side-effects: shader->renderID is initialized to zero
*/
static void GS_CompileProgramShaders(Shader *shader) 
{
    unsigned int vID, fID;
    shader->renderID = 0;
    
    if (GS_CompileShader(&vID, shader->vertexPath, GL_VERTEX_SHADER))
        return;
    
    if (GS_CompileShader(&fID, shader->fragmentPath, GL_FRAGMENT_SHADER))
        return;
        
    shader->renderID = GS_AttachShader(vID, fID);
    return;
}

/*
Side-effects: Deletes 's after linking;
*/
static int GS_CompileShader(GS_ShaderID *renderID, const char *path, GLenum type)
{
    GC_LOG("\nCompiling Shader...\n");
    int logLen, status;
    GC_String *source;
    const char *p;
    *renderID = glCreateShader(type);

    source = GC_ReadFile(path);
    if (!source)
    {
        glDeleteShader(*renderID);
        return 1;
    }
    
    p = source->string;
    glShaderSource(*renderID, 1, &p, NULL);
    glCompileShader(*renderID);
    
    free(source);
    
    glGetShaderiv(*renderID, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        glGetShaderInfoLog(*renderID, 1024, &logLen, infoLog);
        GC_LOG("Shader Compilation Error:\n%s", infoLog);
        
        return 1;
    }

    GC_LOG("Successfully compiled shader!\n");
    return 0;
}

/*
Side-effects: Deletes ShaderID's after linking;
*/
static GS_ProgramID GS_AttachShader(GS_ShaderID vID, GS_ShaderID fID)
{
    int status;
    GS_ProgramID renderID = glCreateProgram();
    
    if (renderID == 0)
    {
        GC_LOG("An error occured while creating program shader\n");
        return 0;
    }

    glAttachShader(renderID, vID);
    glAttachShader(renderID, fID);
    status = GS_LinkProgram(renderID);

    glDeleteShader(vID);
    glDeleteShader(fID);

    if (status)
    {
        glDeleteProgram(renderID);
        renderID = 0;
    }
    
    return renderID;
}

static int GS_LinkProgram(GS_ProgramID renderID)
{
    int status, logLen;
    GC_LOG("\nLinking Shaders...\n");
    
    glLinkProgram(renderID);

    glGetProgramiv(renderID, GL_LINK_STATUS, &status);
    if (!status)
    {
        glGetProgramInfoLog(renderID, 1024, &logLen, infoLog);
        GC_LOG("Shader Program Linking Error:\n%s",infoLog);
        
        return -1; // compilation error
    }

    GC_LOG("Successfully linked shader program!\n");
    return 0; // error;
}