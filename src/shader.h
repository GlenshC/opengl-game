#if !defined(SHADER_H)
#define SHADER_H

/*
    Shader.status
    0 - in use
    1 - successfully compiled
   -1 - an error occured
*/ 
typedef struct {
    unsigned int renderID;
    unsigned int status;
    char *vertexPath;
    char *vragmentPath;

} Shader;

// returns NULL, if compilation error, else check status
Shader *GCShader_CreateShader(const char* vertexPath, const char* fragmentPath);
void GCShader_UseShader(Shader *shader);
void GCShader_SetUniformMat4(Shader *shader, const char *name, void * mat4);
// Shader setUniform(Shader *shader, );

#endif
