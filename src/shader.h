#if !defined(SHADER_H)
#define SHADER_H

#define GS_SHADER_MAX_PATH 512
#define GS_SHADER_MAX_PROGRAMS 256
/*
    Shader.renderID
     0 - an error occured
*/ 

typedef unsigned int GS_ShaderHandle;

GS_ShaderHandle GS_Shader_CreateProgram(const char* vertexPath, const char* fragmentPath);
void GS_Shader_RecompileProgram(const GS_ShaderHandle shader);
void GS_Shader_RecompileAll(void);

void GS_Shader_UseProgram(const GS_ShaderHandle shader);
void GS_Shader_DeleteProgram(const GS_ShaderHandle handle);

void GS_Shader_SetUniformMat4(const GS_ShaderHandle shader, const char *name, const void * Mat4);
void GS_Shader_SetUniformVec3f(const GS_ShaderHandle shader, const char *name, const void *Vec3);
void GS_Shader_SetUniformInt(const GS_ShaderHandle shader, const char *name, const int value);
void GS_Shader_SetUniformFloat(const GS_ShaderHandle shader, const char *name, const float value);


#endif