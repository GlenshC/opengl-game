#if !defined(GC_STRING_H)
#define GC_STRING_H

#define GC_STRING_MAX 1024
typedef struct {
    int len;
    char string[];
} GC_String;

GC_String *GC_ReadFile(const char* path);

#define    GC_strcpy(dest, src, n, max) GC_strcpy_internal(dest, src, n, max, __FILE__, __LINE__)  
void      *GC_strcpy_internal(char *dest, const char *src, int n, int max, const char* file, int line);

#endif
