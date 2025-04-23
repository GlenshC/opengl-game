#if !defined(G_STRING_H)
#define G_STRING_H

typedef struct {
    int len;
    char string[];
} G_String;

G_String *G_ReadFile(const char* path);

#endif
