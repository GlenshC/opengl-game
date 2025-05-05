#ifndef GS_MESH_H
#define GS_MESH_H

#include "types.h"
#include "mesh_types.h"

void GS_Mesh_Create(GSMesh *mesh);          // creates new vectors (only vectors)

void GS_Mesh_Setup(GSMesh *mesh);            // make sure mesh is created first
void GS_Mesh_Draw(GSMesh *mesh, GS_ShaderHandle sHandle);

void GS_Mesh_Empty(GSMesh *mesh);           // sets all vectors size to zero

#define GS_Mesh_Free(mesh) GS_Mesh_Static_Free(mesh) 
GSMesh GS_Mesh_Static_Create();             // creates static struct, and vectors
void GS_Mesh_Static_Free(GSMesh *mesh);     // does not free the GSMesh struct

GSMesh *GS_Mesh_Dynamic_Create();           // creates dynamic struct, and vectors
void GS_Mesh_Dynamic_Free(GSMesh *mesh);    // frees the struct and the vectors

#endif