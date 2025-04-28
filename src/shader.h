#if !defined(SHADER_H)
#define SHADER_H

#define GS_SHADER_MAX_PATH 512
/*
    Shader.renderID
     0 - an error occured
*/ 
typedef struct {
    unsigned int renderID;
    char vertexPath[GS_SHADER_MAX_PATH];
    char fragmentPath[GS_SHADER_MAX_PATH];
} Shader;

typedef unsigned int GS_ProgramID;
typedef unsigned int GS_ShaderID;

// returns NULL, if compilation error, else check status
Shader *GS_Shader_CreateProgram(const char* vertexPath, const char* fragmentPath);
Shader *GS_Shader_GetActiveShader();
void GS_Shader_RecompileProgram(Shader *shader);
void GS_Shader_UseProgram(Shader *shader);

void GS_Shader_SetUniformMat4(Shader *shader, const char *name, void * Mat4);
void GS_Shader_SetUniformVec3f(Shader *shader, const char *name, void * Vec3);

void GS_Shader_SetUniformInt(Shader *shader, const char *name, int value);
void GS_Shader_SetUniformFloat(Shader *shader, const char *name, float value);

#endif
