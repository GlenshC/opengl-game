#include <stdio.h>
#include <stdlib.h>

#include "g_string.h"
#include "glogs.h"

G_String *G_ReadFile(const char* path)
{
    G_String *buffer;    
    FILE *file;
    if (!(file = fopen(path, "r")))
    {
        GLOGS_LOG("Failed to open file %s\n", path);
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

    
    buffer = (G_String *)malloc(sizeof(G_String) + (size_t)len);
    size_t bytesRead = fread(buffer->string, sizeof(char), (size_t)len, file);

    buffer->string[bytesRead] = '\0';
    buffer->len = bytesRead;

    fclose(file);

    GLOGS_LOG("File %s read successfully.\n",path);

    return buffer;
}

/* Implement read batch of file (malloc a batch of file)
G_String *G_ReadMultipleFiles(const char** paths, int n);
*/

// dev notes
// develop an safe input system for strings;
