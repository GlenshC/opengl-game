#if !defined(GLOGS_H)
#define GLOGS_H

#include <stdio.h>
#ifdef GDEBUG
    #define GLOGS_LOG(x, ...) printf(x, ##__VA_ARGS__)
#else
    #define GLOGS_LOG(...)
    // #define printf(...) 
#endif

#endif
