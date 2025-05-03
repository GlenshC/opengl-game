#include <stdlib.h>
#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "gc_string.h"
#include "gc_logs.h"

#ifndef GS_SHADER_MAX_PATH
#define GS_SHADER_MAX_PATH 1024
#endif

typedef struct {
    char vertexPath[GS_SHADER_MAX_PATH];
    char fragmentPath[GS_SHADER_MAX_PATH];
} GSShaderDetails;

typedef unsigned int ProgramIndex; 
typedef unsigned int GSProgramID;
typedef unsigned int GSShaderID;

// Prototypes
static GS_ShaderHandle GS_Shader_CreateHandle(void);
static void GS_Shader_DeleteHandle(const GS_ShaderHandle handle);
static void GS_CompileProgramShaders(const GS_ShaderHandle shader);
static int GS_CompileShader(GSShaderID *renderID, const char *path, const GLenum type);
static GSProgramID GS_AttachShader(const GSShaderID vID, const GSShaderID fID);
static int GS_LinkProgram(const GSProgramID renderID);
static char infoLog[1024];


// Handle System
GSProgramID GS_ACTIVE_PROGRAMS[GS_SHADER_MAX_PROGRAMS]; // active handles
GSShaderDetails GS_ACTIVE_PROGRAM_DETAILS[GS_SHADER_MAX_PROGRAMS]; // active handles details
unsigned int GS_ACTIVE_PROGRAMS_SIZE;

ProgramIndex GS_FREE_PROGRAMS[GS_SHADER_MAX_PROGRAMS]; // deleted handles in active handles (index to a element in active handle)
unsigned int GS_FREE_PROGRAMS_SIZE;

// Handle System Helpers
#define GET_SHADER_OBJECT(handleID) &GS_ACTIVE_PROGRAM_DETAILS[(handleID) - 1] 
#define GET_PROGRAM_ID(handleID) GS_ACTIVE_PROGRAMS[(handleID) - 1]
#define SET_PROGRAM_ID(handleID, id) GS_ACTIVE_PROGRAMS[(handleID) - 1] = id
#define HANDLE_TO_INDEX(handleID) (handleID) - 1 


// Recompiles Shader Program
// incase of error Preserves working shader program
// when successful Replaces old shader program with the new one
void GS_Shader_RecompileProgram(const GS_ShaderHandle handle)
{
    if (handle == 0)
    {
        GC_LOG("Invalid Handle.");
        return;
    }

    unsigned int renderID = GET_PROGRAM_ID(handle); // store previously working renderID;
    GC_LOG("Recompiling Shader...\n");

    GS_CompileProgramShaders(handle); // recompilation
    if (GET_PROGRAM_ID(handle) == 0) {

        GC_LOG("Failed to recompile shader program.\n\n");
        SET_PROGRAM_ID(handle, renderID);

    } else {
        
        GC_LOG("Successfully recompiled shader program.\n\n");
        glDeleteProgram(renderID);
    
    }
}

// When Failure, returns 0
// When Successful, returns a handle to a shader program
// Successfully creating a program doesn't mean you can use that shader;
GS_ShaderHandle GS_Shader_CreateProgram(const char* vertexPath, const char* fragmentPath)
{
    if (vertexPath == NULL || fragmentPath == NULL)
    {
        GC_LOG("Creating Shader Failed: PATH is NULL.\n");
        return 0;
    }

    GC_LOG("Creating Shader...\n");
    GS_ShaderHandle handleID = GS_Shader_CreateHandle();
    if (handleID == 0)
        return 0;
    

    GSShaderDetails *shader = GET_SHADER_OBJECT(handleID);

    GC_STR_strcpy(shader->vertexPath, vertexPath, 0, 512);
    GC_STR_strcpy(shader->fragmentPath, fragmentPath, 0, 512);
    
    GS_CompileProgramShaders(handleID);

    GC_LOG("\nShader Created Succesfully!\n\n");
    
    return handleID;
}

void GS_Shader_UseProgram(const GS_ShaderHandle shader)
{
    if (shader == 0)
    {
        // GC_LOG("UseShader(Shader *shader):\n"
        //        "Shader cannot be used,"
        //        "an error occured while create shader.\n\n");
        return;
    }
    glUseProgram(GET_PROGRAM_ID(shader));
}

void GS_Shader_DeleteProgram(const GS_ShaderHandle handle)
{
    GS_Shader_DeleteHandle(handle);
}


void GS_Shader_SetUniformMat4(const GS_ShaderHandle shader, const char *name, const void * Mat4)
{
    int location = glGetUniformLocation(GET_PROGRAM_ID(shader), name);
    glUniformMatrix4fv(location, 1, GL_FALSE, Mat4);
}

void GS_Shader_SetUniformVec3f(const GS_ShaderHandle shader, const char *name, const void *Vec3)
{
    int location = glGetUniformLocation(GET_PROGRAM_ID(shader), name);
    glUniform3fv(location, 1, Vec3);
}

void GS_Shader_SetUniformInt(const GS_ShaderHandle shader, const char *name, const int value)
{
    int location = glGetUniformLocation(GET_PROGRAM_ID(shader), name);
    glUniform1i(location, value);
}

void GS_Shader_SetUniformFloat(const GS_ShaderHandle shader, const char *name, const float value)
{
    int location = glGetUniformLocation(GET_PROGRAM_ID(shader), name);
    glUniform1f(location, value);
}


/* 
    PRIVATE FUNCTIONS
*/

// When Failure, returns 0
// When Successful, returns a handle to a shader program object  
static GS_ShaderHandle GS_Shader_CreateHandle(void)
{
    if (GS_FREE_PROGRAMS_SIZE)
    {
        return GS_FREE_PROGRAMS[--GS_FREE_PROGRAMS_SIZE];
    }
    
    if (GS_ACTIVE_PROGRAMS_SIZE >= GS_SHADER_MAX_PROGRAMS)
    {
        GC_LOG("Error: Max Handles Achieved. Can't create more Shaders.");
        return 0;
    }
    
    return ++GS_ACTIVE_PROGRAMS_SIZE;
    
}

static void GS_Shader_DeleteHandle(const GS_ShaderHandle handle)
{
    if (handle == GS_ACTIVE_PROGRAMS_SIZE)
    {
        --GS_ACTIVE_PROGRAMS_SIZE;
    } else
    {
        GS_FREE_PROGRAMS[GS_FREE_PROGRAMS_SIZE++] = HANDLE_TO_INDEX(handle);
    }
    SET_PROGRAM_ID(handle, 0);
}

// When successful, programID is set to a valid program;
// When an error occurs, programID is set to zero
static void GS_CompileProgramShaders(const GS_ShaderHandle handle) // make sure handle is not zero
{
    unsigned int vID, fID;
    GSShaderDetails *shader = GET_SHADER_OBJECT(handle);
    
    if (GS_CompileShader(&vID, shader->vertexPath, GL_VERTEX_SHADER))
        return;
    if (GS_CompileShader(&fID, shader->fragmentPath, GL_FRAGMENT_SHADER))
    {
        glDeleteShader(vID);
        return;
    }
    
    SET_PROGRAM_ID(handle, GS_AttachShader(vID, fID)); 
    return;
}

// When error occurs, it automatically dispose the shader;
// Returns 1 when an error occurs
static int GS_CompileShader(GSShaderID *renderID, const char *path, const GLenum type)
{
    GC_LOG("\nCompiling Shader...\n");
    int logLen, status;

    *renderID = glCreateShader(type);

    GC_String *source = GC_STR_ReadFile(path);
    if (!source)
    {
        glDeleteShader(*renderID);
        return 1;
    }
    
    const char *p = source->string;
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

// When Successful creates a new Shader Program and returns it;
// Returns 0 when an error occurs
static GSProgramID GS_AttachShader(const GSShaderID vID, const GSShaderID fID)
{
    int status;
    GSProgramID renderID = glCreateProgram();
    
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

static int GS_LinkProgram(const GSProgramID renderID)
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
    return 0; // normal;
}