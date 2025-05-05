#ifndef GS_MODEL_H
#define GS_MODEL_H

#include "types.h"

typedef gcv_t(GSMesh) GSMeshVector;

typedef struct {
    char path[512];
    GSMeshVector *meshes;
} GSModel;

GSModel *GS_Model_Load(const char *path);
void GS_Model_Free(GSModel *model);

#endif