#include <stdio.h>
#include <stdlib.h>

#include "gc_string.h"
#include "gc_logs.h"

GC_String *GC_STR_ReadFile(const char* path)
{
    GC_String *buffer;    
    FILE *file;
    if (!(file = fopen(path, "r")))
    {
        GC_LOG("Failed to open file %s\n", path);
        return NULL;
    }
    long len;
    
    fseek(file, 0, SEEK_END);
    len = ftell(file);
    if (len < 0)
    {
        fclose(file);
        return NULL;
    }
    fseek(file, 0, SEEK_SET);

    
    buffer = (GC_String *)malloc(sizeof(GC_String) + (size_t)len);
    size_t bytesRead = fread(buffer->string, sizeof(char), (size_t)len, file);

    buffer->string[bytesRead] = '\0';
    buffer->len = bytesRead;

    fclose(file);

    GC_LOG("File %s read successfully.\n",path);

    return buffer;
}

void GC_STR_strcpy_internal(char *dest, const char *src, int n, int max, const char* file, int line) 
{
    do {
        if (n >= max) {
            *(dest+(n-1)) = 0;
            GC_LOG("%s:%d: BUFFER_OVERFLOW: Surpassed limit %d bytes\n", file, line, max);
            break;
        }

        *(dest+(n++)) = *(src++);
    } while (*src);

    *(dest+(n)) = 0;
}

/* Implement read batch of file (malloc a batch of file)
G_String *G_ReadMultipleFiles(const char** paths, int n);
*/

// dev notes
// develop an safe input system for strings;
