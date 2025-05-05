#ifndef GS_TYPES_H
#define GS_TYPES_H

#include "cglm/types.h"
typedef unsigned int GLenum;
typedef unsigned int GS_ShaderHandle;
typedef unsigned int GSTextureHandle;

typedef enum GSTextureType{
    GS_TexDiffuse,
    GS_TexSpecular
} GSTextureType;

typedef struct {
    GSTextureHandle handle;
    GSTextureType type;
} GSTexture;

#endif