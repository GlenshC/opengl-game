#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#include "gc_logs.h"

#ifndef MAX_TEXTURES
#define MAX_TEXTURES 1024
#endif

typedef enum GSTextureType{
    TEXTURE2D,
    TEXTURE3D
} GSTextureType;

typedef unsigned char BYTE;
typedef unsigned int GSTextureID;
typedef struct {
    GSTextureID textures[MAX_TEXTURES];
    GSTextureType type[MAX_TEXTURES];
    size_t size;
} TEXTURES;

static TEXTURES TEXTURE_HANDLES;

#define POP_TEXTURE_HANDLE --TEXTURE_HANDLES.size
#define PUSH_TEXTURE_HANDLE ++TEXTURE_HANDLES.size
#define GET_TEXTURE_ID(handle) TEXTURE_HANDLES.textures[(handle) - 1]
#define SET_TEXTURE_ID(handle, id) TEXTURE_HANDLES.textures[(handle) - 1] = id
#define HANDLE_TO_INDEX(handle) ((handle) - 1)

static GSTextureHandle 
TextureHandle_create()
{
    if (TEXTURE_HANDLES.size >= MAX_TEXTURES)
        return 0;

    return PUSH_TEXTURE_HANDLE;
}

static GSTextureID 
TextureHandle_getID(GSTextureHandle handle)
{
    if (handle == 0)
        return 0;

    return GET_TEXTURE_ID(handle);
}

static void 
TextureHandle_setID(GSTextureHandle handle, GSTextureID id)
{
    if (handle == 0)
        return;
    
    SET_TEXTURE_ID(handle, id);
}

static void 
TextureHandle_delete(GSTextureHandle handle)
{
    if (handle == 0)
        return;
    
    SET_TEXTURE_ID(handle, TEXTURE_HANDLES.textures[POP_TEXTURE_HANDLE]);
}


GSTextureHandle 
GS_GenTexture2D(char const *path, int alpha)
{
    GSTextureHandle handle = TextureHandle_create();
    GSTextureID id;
    int x, y, comp;
    GLenum colorFormat = GL_RGB;
    // stbi_set_flip_vertically_on_load(1);
    BYTE *textureData = stbi_load(path, &x, &y, &comp, 0); 
    if (!textureData)
    {
        GC_LOG("Error in loading the texture.\n");
        return 0;
    }

    if (alpha) {
        colorFormat = GL_RGBA;
    }

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, x, y, 0, colorFormat, GL_UNSIGNED_BYTE, textureData);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(textureData);

#ifdef GS_TEXTURE_DEFAULT_PARAMETERS
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#endif

    TextureHandle_setID(handle, id);
    return handle;
}

void 
GS_ActiveTexture(unsigned int index, GLenum texture_type, GSTextureHandle handle) 
{
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(texture_type, TextureHandle_getID(handle));
}

void 
GS_SetTextureWrap(GLenum target, GLenum type)
{
    glTexParameteri(target, GL_TEXTURE_WRAP_S, type);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, type);
}

void 
GS_SetTextureFilter(GLenum target, GLenum min, GLenum max)
{
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, max);
}