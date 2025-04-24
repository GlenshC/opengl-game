#if !defined(GCLOGS_H)
#define GCLOGS_H

#include <stdio.h>

#ifdef GC_DEBUG
    #define GC_LOG(x, ...) printf(x, ##__VA_ARGS__)
#else
    #define GC_LOG(...)
    // #define printf(...) 
#endif

#endif
