
#ifndef GS_MAIN_H
#define GS_MAIN_H

#ifndef offsetof
#   define offsetof(TYPE, ELEMENT) ((size_t) &(((TYPE *)0)->ELEMENT))
#   ifdef __has_builtin
#       if __has_builtin(__builtin_offsetof)
#           undef offsetof
#           define offsetof(TYPE, MEMBER) __builtin_offsetof(TYPE, MEMBER)
#       endif
#   endif
#endif

#endif

