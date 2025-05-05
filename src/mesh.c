#include <glad/glad.h>

#include "gc_logs.h"
#include "texture.h"
#include "shader.h"
#include "mesh.h"
#define GC_VECTOR_SHORT_NAMES
#include "gc_vector.h"

#include "shader_variables.h"

static void GS_Mesh_SetupBuffers(GSMesh *mesh);

// creates new vectors (only vectors)
void GS_Mesh_Create(GSMesh *mesh)
{
    mesh->indices = gcv_create(mesh->indices);
    mesh->textures = gcv_create(mesh->textures);
    mesh->vertices = gcv_create(mesh->vertices);
}

// sets all vectors size to zero
void GS_Mesh_Empty(GSMesh *mesh)
{
    gcv_empty(mesh->vertices);
    gcv_empty(mesh->textures);
    gcv_empty(mesh->indices);
}

void GS_Mesh_Setup(GSMesh *mesh)
{
    if (mesh->indices == NULL)
    {
        GC_LOG("Mesh not initialized.\n");
        return;
    }

    GS_Mesh_SetupBuffers(mesh);
}

void GS_Mesh_Draw(GSMesh *mesh, GS_ShaderHandle sHandle)
{
    unsigned int diffuseN = 1;
    unsigned int specularN = 1;
    char *name;
    char buffer[256];
    for (unsigned int i =0; i < gcv_size(mesh->textures); i++)
    {
        if (gcv_at(mesh->textures, i).type == GS_TexDiffuse)
            snprintf(buffer, 256, "material.%s%u",  TEXTURE_DIFFUSE, diffuseN++);
        else 
            snprintf(buffer, 256, "material.%s%u", TEXTURE_SPECULAR, specularN++);
        
        GS_Shader_SetUniformInt(sHandle, buffer, i);
        GS_ActiveTexture(i, GL_TEXTURE_2D, gcv_at(mesh->textures, i).handle);
    }
}


/* Memory Dependent Functions */

// frees the struct and the vectors
void GS_Mesh_Dynamic_Free(GSMesh *mesh)
{
    GS_Mesh_Static_Free(mesh);
    free(mesh);
}

// does not free the GSMesh struct
void GS_Mesh_Static_Free(GSMesh *mesh)
{
    gcv_free(mesh->vertices);
    gcv_free(mesh->textures);
    gcv_free(mesh->indices);
}

// creates dynamic struct, and vectors
GSMesh *GS_Mesh_Dynamic_Create()
{
    GSMesh *mesh = malloc(sizeof(GSMesh));
    GS_Mesh_Create(mesh);
    return mesh;
}

// creates static struct, and vectors
GSMesh GS_Mesh_Static_Create()
{
    GSMesh mesh;
    GS_Mesh_Create(&mesh);
    return mesh;
}


// Private Functions
static void GS_Mesh_SetupBuffers(GSMesh *mesh)
{
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, gcv_size(mesh->vertices) * sizeof(GSVertex),
                 &gcv_data(mesh->vertices), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, gcv_size(mesh->indices) * sizeof(unsigned int),
                 &gcv_data(mesh->indices), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GSVertex), (void *) (0) );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GSVertex), 
                          (void *) (offsetof(GSVertex, normal)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GSVertex), 
                          (void *) (offsetof(GSVertex, texCoords)));
    glEnableVertexAttribArray(2);
    
    
    mesh->VAO = VAO;
    mesh->VBO = VBO;
    mesh->EBO = EBO;
}