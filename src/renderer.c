#include "renderer.h"

#include "gc_logs.h"
#include "cglm/cglm.h"

#ifndef RENDERER_MAX_ENTITIES
#define RENDERER_MAX_ENTITIES 1024
#endif

typedef struct
{
    unsigned int VBO;
    unsigned int VAO;
    unsigned int vertices;
} Mesh;


typedef struct 
{
    vec3 position[RENDERER_MAX_ENTITIES];
    vec3 rotation[RENDERER_MAX_ENTITIES];
    vec3 scale[RENDERER_MAX_ENTITIES];
    unsigned int mesh[RENDERER_MAX_ENTITIES]; // if valid
    unsigned int material[RENDERER_MAX_ENTITIES]; // if valid
    size_t size;
} Entities;


Entities ACTIVE_ENTITIES;



void GS_Renderer_CreateEntity()
{
    isValidShader();
    isValidTexture();
}

void GS_Renderer_Kill(Entity id)
{
    
}

