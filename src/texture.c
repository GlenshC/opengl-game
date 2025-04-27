#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#include "gc_logs.h"

typedef unsigned char BYTE;

GSTexture GS_GenTexture2D(char const *path, GLenum colorFormat)
{
    GSTexture id;
    int x, y, comp;
    stbi_set_flip_vertically_on_load(1);
    BYTE *textureData = stbi_load(path, &x, &y, &comp, 0); 
    if (!textureData)
    {
        GC_LOG("Error in loading the texture.\n");
        return 0;
    }

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, colorFormat, GL_UNSIGNED_BYTE, textureData);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(textureData);

#ifdef GS_TEXTURE_DEFAULT_PARAMETERS
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#endif

    return id;
}

void GS_ActiveTexture(unsigned int index, GLenum texture_type, GSTexture texture) 
{
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(texture_type, texture);
}

void GS_SetTextureWrap(GLenum target, GLenum type)
{
    glTexParameteri(target, GL_TEXTURE_WRAP_S, type);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, type);
}

void GS_SetTextureFilter(GLenum target, GLenum min, GLenum max)
{
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, max);
}