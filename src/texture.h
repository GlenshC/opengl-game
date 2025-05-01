#if !defined(TEXTURE_H)
#define TEXTURE_H

typedef unsigned int GSTextureHandle;

#define GS_TEXTURE_DEFAULT_PARAMETERS
#include <glad/glad.h>

// if you want to avoid settings texture parameters just add the ff flags while compiling:
// -DGS_TEXTURE_DEFAULT_PARAMETERS
#define MAX_TEXTURES 256

GSTextureHandle GS_GenTexture2D(char const *path, int alpha);
void GS_ActiveTexture(unsigned int index, GLenum texture_type, GSTextureHandle handle);
void GS_SetTextureWrap(GLenum target, GLenum type);
void GS_SetTextureFilter(GLenum target, GLenum min, GLenum max);

#endif
