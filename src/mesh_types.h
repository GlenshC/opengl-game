
#ifndef GS_MESH_TYPES_H
#define GS_MESH_TYPES_H

#include "types.h"
#define GC_VECTOR_SHORT_NAMES
#include "gc_vector.h"

typedef struct GSVertex {
    vec3 position;
    vec3 normal;
    vec3 texCoords;
} GSVertex;

typedef gcv_t(GSVertex) GSVertexVector;
typedef gcv_t(unsigned int) GSUintVector;
typedef gcv_t(GSTexture) GSTextureVector;

typedef struct GSMesh
{
    GSVertexVector *vertices;
    GSUintVector *indices;
    GSTextureVector *textures;
    unsigned int VAO, VBO, EBO;
} GSMesh;

#endif // GS_MESH_TYPES_H included