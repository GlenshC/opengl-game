#if !defined(GC_STRING_H)
#define GC_STRING_H

#define GC_STRING_MAX 1024
typedef struct {
    int len;
    char string[];
} GC_String;

GC_String *GC_STR_ReadFile(const char* path);
void      *GC_STR_strcpy_internal(char *dest, const char *src, int n, int max, const char* file, int line);

#define    GC_STR_strcpy(dest, src, n, max) GC_STR_strcpy_internal(dest, src, n, max, __FILE__, __LINE__)  

#endif
